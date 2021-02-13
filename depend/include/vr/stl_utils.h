/*
* vrbase
* Copyright 2007 CENPES/Petrobras Tecgraf/PUC-Rio
*/

#ifndef _VR_STL_UTILS_H_
#define _VR_STL_UTILS_H_

#include <vr/common.h>
#include <vector>

/*!
	\file stl_utils.h Contains utility functions to manipulate objects from the Standard Template Library.
*/

namespace vr {

/*!
	Sets new capacity equal to new size, do not grow vector by half.
	Used when exact number of elements is previously known, optimizing memory allocation.
*/
template<typename T>
inline static void vectorExactResize( std::vector<T>& v, uint32 newSize )
{
	v.reserve( newSize );
	v.resize( newSize );
}

/*!
	Sets new capacity equal to new size, do not grow vector by half.
	Used when exact number of elements is previously known, optimizing memory allocation.
	Parameter <tt>pad</tt> is used for new elements added if the new size is larger that the original size.
*/
template<typename T>
inline static void vectorExactResize( std::vector<T>& v, uint32 newSize, T pad )
{
	v.reserve( newSize );
	v.resize( newSize, pad );
}

/*!
	The following need to temporarily duplicate vector memory storage.
	They are not recommended for very large vectors.
*/

//! Sets vector to interval [<tt>first</tt>, <tt>last</tt>], erases all other data
template<typename T>
inline static void vectorClip( std::vector<T>& v, uint32 first, uint32 last )
{
	std::vector<T>( v.begin() + first, v.begin() + last ).swap( v );
}

//! Sets capacity equal to current size. Free unused memory.
template<typename T>
inline static void vectorTrim( std::vector<T>& v )
{
	std::vector<T>( v ).swap( v );
}

//! Sets size and capacity to zero. Free all memory.
template<typename T>
inline static void vectorFreeMemory( std::vector<T>& v )
{
	std::vector<T>().swap( v );
}

} // namespace vr

#endif // _VR_STL_UTILS_H_
