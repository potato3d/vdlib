#ifndef _VR_PLATFORM_H_
#define _VR_PLATFORM_H_

#if defined(_MSC_VER)
	#pragma warning( disable : 4244 )
	#pragma warning( disable : 4251 )
	#pragma warning( disable : 4267 )
	#pragma warning( disable : 4275 )
	#pragma warning( disable : 4290 )
	#pragma warning( disable : 4786 )
	#pragma warning( disable : 4305 )
	#pragma warning( disable : 4996 )
#endif

// Define NULL pointer value
#ifndef NULL
	#ifdef __cplusplus
		#define NULL    0
	#else
		#define NULL    ((void *)0)
	#endif
#endif

// Portable shared-library interface attributes:
#if defined(_MSC_VER)
	#if defined( BUILDING_VRBASE_LIBRARY )
		#define VRBASE_EXPORT __declspec(dllexport)
	#else
		#define VRBASE_EXPORT __declspec(dllimport)
	#endif
#else
	#define VRBASE_EXPORT
#endif

// Portable macro to 'force' inlining of a function
#if defined(_MSC_VER)
	#define VR_FORCEINLINE	__forceinline
#elif defined(__GNUG__)
	#define VR_FORCEINLINE	inline __attribute__((always_inline))
#else
	#error Oops! The VR_FORCEINLINE macro was not defined for this compiler!
#endif

#if defined(__GNUG__)
	#include <sys/types.h>
#endif

namespace vr {

// Portable integer types:
#if defined(_MSC_VER)
	typedef __int8				int8;
	typedef __int16				int16;
	typedef __int32				int32;
	typedef __int64				int64;
	typedef unsigned __int8		uint8;
	typedef unsigned __int16	uint16;
	typedef unsigned __int32	uint32;
	typedef unsigned __int64	uint64;
#elif defined(__GNUG__)
	typedef int8_t				int8;
	typedef int16_t				int16;
	typedef int32_t				int32;
	typedef int64_t				int64;
	typedef u_int8_t			uint8;
	typedef u_int16_t			uint16;
	typedef u_int32_t			uint32;
	typedef u_int64_t			uint64;
#else
	#error Oops! The portable integer types were not defined for this platform!
#endif

// Portable limits for integer types:
// (current definitions are only valid for x86 platforms)
const int8		INT8_MIN	= -127 - 1;
const int8		INT8_MAX	=  127;
const uint8		UINT8_MAX	=  0xFF;

const int16		INT16_MIN	= -32767 - 1;
const int16		INT16_MAX	=  32767;
const uint16	UINT16_MAX	= 0xFFFF;

const int32		INT32_MIN	= -2147483647 - 1;
const int32		INT32_MAX	=  2147483647;
const uint32	UINT32_MAX	=  0xFFFFFFFF;

#if defined(_MSC_VER)
const int64		INT64_MAX	= 0x7FFFFFFFFFFFFFFF;
const uint64	UINT64_MAX	= 0xFFFFFFFFFFFFFFFF;
#else
const int64		INT64_MAX	= 0x7FFFFFFFFFFFFFFFLL;
const uint64	UINT64_MAX	= 0xFFFFFFFFFFFFFFFFULL;
#endif
const int64		INT64_MIN	= -INT64_MAX-1;

} // namespace vr

/*!
	Imports all portable integer types into the current namespace.
	This is useful if you want to write <tt>uint8</tt>, <tt>int32</tt>, etc. in your
	program but don't want to fully import the vr namespace.
 */
#define USING_VR_INTEGER_TYPES	\
	using vr::int8;		\
	using vr::int16;	\
	using vr::int32;	\
	using vr::int64;	\
	using vr::uint8;	\
	using vr::uint16;	\
	using vr::uint32;	\
	using vr::uint64;

#endif
