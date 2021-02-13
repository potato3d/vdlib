#ifndef _VR_COMMON_H_
#define _VR_COMMON_H_

#ifndef __cplusplus
	#error The vr base library requires a C++ compiler.
#endif

#include <vr/platform.h>

/*!
	Evaluates a constant expression and, if the result is false, aborts the
	compilation with an error message.

	\param const_expr is a compile-time integral or pointer expression.
	\param id_msg is a C++ identifier that describes the error (it does not
		need to be defined). Something like 'invalid_element_size'.
 */
#define VR_STATIC_CHECK( const_expr, id_msg ) \
	{ vr::CompileTimeError<( (const_expr) != 0 )> ERROR_##id_msg; (void)ERROR_##id_msg; }

/*!
	Utility macro that returns the number of elements in an array.
	Useful when using dynamically-sized arrays.
 */
#define VR_ARRAY_LENGTH( array )	(sizeof(array)/sizeof(array[0]))

namespace vr {

// Template trick to yield compile time errors:
template<int> struct CompileTimeError;
template<> struct CompileTimeError<true> {};

} // namespace vr

#endif
