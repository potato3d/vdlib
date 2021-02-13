/*
 * vrbase
 * Copyright 2007 CENPES/Petrobras Tecgraf/PUC-Rio
 */

#ifndef _VR_STRING_H_
#define _VR_STRING_H_

#include <vr/common.h>
#include <vector>

//! Efficiently builds a vr::String from a literal string.
#define VR_LITERAL_STRING( ls ) vr::String( ( reinterpret_cast<const void*>( "" ls "" ) ), ( sizeof( ls ) - 1 ) )

extern "C" {

#ifndef BSTRLIB_INCLUDE
    struct tagbstring {
        int mlen;
        int slen;
        unsigned char * data;
    };
#endif

    typedef int (*CharGetter) (void *parm);
    typedef size_t (* StreamReader) (void *buff, size_t elsize, size_t nelem, void *parm);
}

namespace vr {

class String;
class StringList;

/*!
    Reference to a character in a String.
    Enforces that write-protected strings are not changed.
 */
class VRBASE_EXPORT CharWriteProtected
{
    friend class String;

public:
    char operator =( char c );
    unsigned char operator =( unsigned char c );
    inline operator unsigned char() const
    {
        return _str.data[_idx];
    }

private:
    CharWriteProtected( const tagbstring& str, int i );

private:
    const tagbstring& _str;
    unsigned int _idx;
};

/*!
    An 8-bit clean string class.
 */
class VRBASE_EXPORT String : public tagbstring
{
public:
    static const String BLANK_CHARS;
	static const int ERR;

public:
    // General Constructors
    String();
    String( char c );
    String( unsigned char c );
    String( const char* s );
    String( int len, const char* s );
    String( const String& b );
    String( const tagbstring& x );
    String( char c, int len );
    String( const void* blk, int len );

    // Constructors that join string lists
    String( const StringList& l );
    String( const StringList& l, const String& sep );
    String( const StringList& l, char sep );
    String( const StringList& l, unsigned char sep );

    // Destructor
    virtual ~String();

    // = operator
    const String& operator =( char c );
    const String& operator =( unsigned char c );
    const String& operator =( const char* s );
    const String& operator =( const String& b );
    const String& operator =( const tagbstring& x );

    // += operator
    const String& operator +=( char c );
    const String& operator +=( unsigned char c );
    const String& operator +=( const char* s );
    const String& operator +=( const String& b );
    const String& operator +=( const tagbstring& x );

    // + operator
    const String operator +( char c ) const;
    const String operator +( unsigned char c ) const;
    const String operator +( const unsigned char* s ) const;
    const String operator +( const char* s ) const;
    const String operator +( const String& b ) const;
    const String operator +( const tagbstring& x ) const;

    // Comparison operators
    bool operator ==( const String& b ) const;
    bool operator ==( const char* s ) const;
    bool operator ==( const unsigned char* s ) const;
    bool operator !=( const String& b ) const;
    bool operator !=( const char* s ) const;
    bool operator !=( const unsigned char* s ) const;
    bool operator <( const String& b ) const;
    bool operator <( const char* s ) const;
    bool operator <( const unsigned char* s ) const;
    bool operator <=( const String& b ) const;
    bool operator <=( const char* s ) const;
    bool operator <=( const unsigned char* s ) const;
    bool operator >( const String& b ) const;
    bool operator >( const char* s ) const;
    bool operator >( const unsigned char* s ) const;
    bool operator >=( const String& b ) const;
    bool operator >=( const char* s ) const;
    bool operator >=( const unsigned char* s ) const;

    // Casts
    inline const char* toCharArray() const
    {
        return reinterpret_cast<char *>( data );
    }

    inline const unsigned char* toByteArray() const
    {
        return data;
    }

	/*!
	  \brief  Converts the string to double
	  \return The converted number. Throws and exception
	          if conversion fails
	*/
    double toDouble() const;

	/*!
	  \brief  Converts the string to float
	  \return The converted number. Throws and exception if
	          conversion fails
	*/
	float toFloat() const;

	/*!
	  \brief  Converts the string to int
	  \return The converted number. Throws and exception if
	          conversion fails
	*/
    int toInt() const;

	/*!
	  \brief  Converts the string to int
	  \return The converted number. Throws and exception if
	          conversion fails
	*/
    unsigned int toUInt() const;

    // Accessors
    inline int length() const
    {
        return slen;
    }

    inline bool isEmpty() const
    {
    	return slen == 0;
    }

    unsigned char at( int i ) const;

    inline unsigned char operator []( int i ) const
    {
        return at( i );
    }

    inline CharWriteProtected at( int i )
    {
        return CharWriteProtected( *this, i );
    }

    inline CharWriteProtected operator []( int i )
    {
        return at( i );
    }

    // Space allocation hint method.
    void reserve( int length );

    // Search methods.
    bool isEqualCI( const String& b ) const;

	/*!
	  \brief   Compares current string to another one, case-insensitivelly.
	  \param   b is the string being compared
	  \return  The return value is the difference of the values of the
	           characters where the two strings first differ after lower
			   case transformation, otherwise 0 is returned indicating that
			   the strings are equal.
	*/
    int compareCI( const String& b ) const;

	bool startsWithCI( vr::String& b ) const;

    int find( char c, int pos = 0 ) const;
    int find( const char* b, int pos = 0 ) const;
    int find( const String& b, int pos = 0 ) const;
    int findCI( const String& b, int pos = 0 ) const;
    int findCI( const char* b, int pos = 0 ) const;
    int findLast( char c, int pos ) const;
    int findLast( const char* b, int pos ) const;
    int findLast( const String& b, int pos ) const;
    int findLastCI( const String& b, int pos ) const;
    int findLastCI( const char* b, int pos ) const;
    int findFirstOfChars( const String& b, int pos = 0 ) const;
    int findFirstOfChars( const char* s, int pos = 0 ) const;
    int findLastOfChars( const String& b, int pos ) const;

	/*!
	  \brief Search for the last position in the string containing the characters in s.
	  \param s is the string containing to be searched in this string.
	  \return the position of the character found or BSTR_ERR.
	 */
	int findLastOfChars( const char* s ) const;

	/*!
	  \brief Search for the last position in the string containing the characters in s.
	  \param s is the string containing to be searched in this string.
	  \param pos is the last character position to be searched.
	  \return the position of the character found or BSTR_ERR.
	 */
    int findLastOfChars( const char* s, int pos ) const;

    int findFirstNotOfChars( const String& b, int pos = 0 ) const;
    int findFirstNotOfChars( const char* b, int pos = 0 ) const;
    int findLastNotOfChars( const String& b, int pos ) const;
    int findLastNotOfChars( const char* b, int pos ) const;

    // Search and substitute methods.
    void replace( const String& find, const String& repl, int pos = 0 );
    void replace( const String& find, const char* repl, int pos = 0 );
    void replace( const char* find, const String& repl, int pos = 0 );
    void replace( const char* find, const char* repl, int pos = 0 );
    void replaceCI( const String& find, const String& repl, int pos = 0 );
    void replaceCI( const String& find, const char* repl, int pos = 0 );
    void replaceCI( const char* find, const String& repl, int pos = 0 );
    void replaceCI( const char* find, const char* repl, int pos = 0 );

    // Substring Extraction method.
    const String substr( int pos, int len ) const;

    // Standard manipulation methods.
    void assign( int pos, const String& b, unsigned char fill = ' ' );
    void assign( int pos, const char* b, unsigned char fill = ' ' );
    void assign( int pos, int len, const String& b, unsigned char fill = ' ' );
    void assign( int pos, int len, const char* s, unsigned char fill = ' ' );
    void insert( int pos, const String& b, unsigned char fill = ' ' );
    void insert( int pos, const char* b, unsigned char fill = ' ' );
    void insertChar( int pos, int times, unsigned char fill = ' ' );
    void remove( int pos, int len );
    void truncate( int len );

    // Miscellaneous methods.
    void format( const char* fmt, ... );
    void appendFormatted( const char* fmt, ... );
    void fill( int length, unsigned char fill = ' ' );
    void repeat( int count );
    void trimLeft( const String& b = BLANK_CHARS );
    void trimRight( const String& b = BLANK_CHARS );
    inline void trim( const String& b = BLANK_CHARS )
    {
        trimRight( b );
        trimLeft( b );
    }
    void convertToUppercase();
    void convertToLowercase();

    // Write protection methods.
    void writeProtect();
    void writeAllow();
    inline bool isWriteProtected() const
    {
        return mlen <= 0;
    }

    // Join methods.
    void join( const StringList& l );
    void join( const StringList& l, const String& sep );
    void join( const StringList& l, char sep );
    void join( const StringList& l, unsigned char sep );

    // StringStream methods.
    int gets( CharGetter getterPtr, void* parm, char terminator = '\n' );
    int read( StreamReader readerPtr, void* parm );
};

VRBASE_EXPORT const String operator +( const char* a, const String& b );
VRBASE_EXPORT const String operator +( const unsigned char* a, const String& b );
VRBASE_EXPORT const String operator +( char c, const String& b );
VRBASE_EXPORT const String operator +( unsigned char c, const String& b );
VRBASE_EXPORT const String operator +( const tagbstring& x, const String& b );

VRBASE_EXPORT std::ostream& operator <<( std::ostream& sout, const String& b );
VRBASE_EXPORT std::istream& operator >>( std::istream& sin, String& b );
VRBASE_EXPORT std::istream& getline( std::istream& sin, String& b, char terminator = '\n' );

/*!
    Wraps a lower-level stream that's read from a function.
 */
class VRBASE_EXPORT StringInputStream
{
    friend class StringList;

public:
    StringInputStream( StreamReader readPtr, void* parm );
    ~StringInputStream();

    int setBufferLength( int sz );
    int getBufferLength();

    int eof() const;

    String readLine( char terminator );
    String readLine( const String& terminator );
    void readLine( String& s, char terminator );
    void readLine( String& s, const String& terminator );
    void readLineAppend( String& s, char terminator );
    void readLineAppend( String& s, const String& terminator );

    String read();
    String& operator >>( String& s );

    String read( int n );
    void read( String& s );
    void read( String& s, int n );
    void readAppend( String& s );
    void readAppend( String& s, int n );

    void unread( const String& s );
    inline StringInputStream& operator <<( const String& s )
    {
        this->unread( s );
        return *this;
    }

    String peek() const;
    void peek( String& s ) const;
    void peekAppend( String& s ) const;

private:
    struct bStream* _stream;
};

/*!
    \brief List of strings.
    Offers methods to split strings.
 */
class VRBASE_EXPORT StringList : public std::vector<String>
{
public:
    void splitAt( const String& b, unsigned char separator, bool keepEmptyParts = false );
    void splitAt( const String& b, const String& separator );
    void splitAtAnyOfChars( const String& b, const String& separators );

    void splitAt( const StringInputStream& b, unsigned char separator );
    void splitAt( const StringInputStream& b, const String& separator );
    void splitAtAnyOfChars( const StringInputStream& b, const String& separators );

	// Comparison operators
	bool operator ==( const StringList& b ) const;
};

} // namespace vr

#endif
