
#ifndef _VR_MEMORYLEAK_H_
#define _VR_MEMORYLEAK_H_

/*!
   Memory Leak Test Class

   @brief Used to test for memory leak problems. Use the macro VR_BEGIN_MEMORY_LEAK_TEST to
   initialize a memory leak test and the macro VR_END_MEMORY_LEAK_TEST to finalize it. If a
   memory leak occur, then it will report the memory usage and trigger a failure using cppunit.

   Follows a sample of how to detect memory leak integrated with cppunit:
   #include <cppunit/extensions/HelperMacros.h>

   #include <tdgnIO/Reader.h>
   #include "memory_leak.h"

   class MemoryLeakTests : public CppUnit::TestFixture
   {
		CPPUNIT_TEST_SUITE( MemoryLeakTests );
		CPPUNIT_TEST( test1MemoryLeak );
		CPPUNIT_TEST_SUITE_END();

	  public:

		void test1MemoryLeak()
		{
			BEGIN_MEMORY_LEAK_TEST;
			malloc(10);
           END_MEMORY_LEAK_TEST;
		}
   };

   CPPUNIT_TEST_SUITE_REGISTRATION( MemoryLeakTests );

 */

#if defined(_MSC_VER)

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


#ifndef _DEBUG											

#define VR_BEGIN_MEMORY_LEAK_TEST

#else

#define VR_BEGIN_MEMORY_LEAK_TEST  								\
	_CrtMemState __initialState;								\
	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );			\
	_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );		\
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );			\
	_CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );		\
	/*_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );*/	\
	/*_CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );*/	\
	_CrtMemCheckpoint( &__initialState ); {						\

#endif															


#ifdef _DEBUG

#define VR_END_MEMORY_LEAK_TEST																		\
	}																								\
	_CrtMemState __finalState, __diffState;															\
	_CrtMemCheckpoint( &__finalState );																\
	int __hasMemoryLeak = _CrtMemDifference( &__diffState, &__initialState, &__finalState );		\
	if ( __hasMemoryLeak )																			\
		_CrtMemDumpStatistics( &__diffState );														\
	CPPUNIT_ASSERT( __hasMemoryLeak == 0 ); 														

#else

#define VR_END_MEMORY_LEAK_TEST

#endif

#else

#endif // Platform definition

#endif // _VR_MEMORYLEAK_H_
