#ifndef _VR_MATH_H_
#define _VR_MATH_H_

#include <vr/common.h>
#include <cmath>
#include <cfloat>

// undefine the min/max macros (usually defined in windows.h)
#ifdef min
	#undef min
#endif
#ifdef max
	#undef max
#endif
#ifndef NOMINMAX
	#define NOMINMAX
#endif

// undefine the 'PI' macro if it was defined by other libraries
#ifdef PI
#undef PI
#undef PI_2
#undef PI_4
#endif

namespace vr {

template<typename real>
class Math
{
public:
	static VRBASE_EXPORT const real PI;				//!< PI
	static VRBASE_EXPORT const real TWO_PI;			//!< 2*PI
	static VRBASE_EXPORT const real PI_2;			//!< PI/2
	static VRBASE_EXPORT const real PI_4;			//!< PI/4
	static VRBASE_EXPORT const real INV_PI;			//!< 1/PI
	static VRBASE_EXPORT const real LN_2;			//!< ln(2)
	static VRBASE_EXPORT const real LN_10;			//!< ln(10)
	static VRBASE_EXPORT const real INV_LN_2;		//!< 1/ln(2)
	static VRBASE_EXPORT const real INV_LN_10;		//!< 1/ln(10)
	static VRBASE_EXPORT const real DEG_TO_RAD;		//!< Degrees to radians ratio.
	static VRBASE_EXPORT const real RAD_TO_DEG;		//!< Radians to degrees ratio.
	static VRBASE_EXPORT const real ZERO_TOLERANCE;	//!< A value that is very close to zero.
	static VRBASE_EXPORT const real EPSILON;		//!< Smallest difference between two numbers.
	static VRBASE_EXPORT const real MAX_VALUE;		//!< Maximum valid value for a number.
	static VRBASE_EXPORT const real MIN_VALUE;		//!< Minimum valid value for a number.
};

//! Returns the base 2 logarithm of a number.
template<typename real>
inline real log2( real scalar )
{
	return log( scalar ) * Math<real>::INV_LN_2;
}

//! Returns the base 10 logarithm of a number.
template<typename real>
inline real log10( real scalar )
{
	return log( scalar ) * Math<real>::INV_LN_10;
}

//! Returns true if the given floating-point value is not a number (NaN).
template<typename real>
inline bool isNaN( real scalar )
{
#if defined(_MSC_VER)
	return _isnan( scalar ) != 0;
#else
	return isnan( scalar );
#endif
}

//! Returns true if the given floating-point value is either infinite (INF) or not a number (NaN).
template<typename real>
inline bool isInvalidNumber( real scalar )
{
#if defined(_MSC_VER)
	return _finite( scalar ) == 0;
#else
	return finite( scalar ) == 0;
#endif
}

//! Compares two floating-point values using a given tolerance.
template<typename real>
inline bool isEqual( real a, real b, real tolerance = Math<real>::EPSILON )
{
	real delta = b - a;
	return ( delta < 0 ? delta >= -tolerance : delta <= tolerance );
}

//! Converts an angle from degrees to radians
template<typename real>
inline real toRadians( real degrees )
{
	return degrees * Math<real>::DEG_TO_RAD;
}

//! Converts an angle from radians to degrees
template<typename real>
inline real toDegrees( real radians )
{
	return radians * Math<real>::RAD_TO_DEG;
}

//! Returns the absolute value of a number -- its value without regard to sign.
template<typename real>
inline real abs( real a )
{
	return ( a >= 0  ? a : -a );
}

//! Returns the minimum of two values.
template<typename real>
inline real min( real a, real b )
{
	return ( a < b ? a : b );
}

//! Returns the maximum of two values.
template<typename real>
inline real max( real a, real b )
{
	return ( a > b ? a : b );
}

//! Returns the fraction digits of the given floating point value. The result is always positive.
template<typename real>
inline real frac( real a )
{
	return ( a >= 0 ) ? a - (int)a : frac( -a );
}

//! Rounds to the nearest integer. Ex: 1.2 becomes 1, 2.5 becomes 3, etc.
template<typename real>
inline real round( real a )
{
	return ( frac( a ) < static_cast<real>( 0.5 ) ) ? floor( a ) : ceil( a );
}

//! Clamps a value to the interval [minimum, maximum].
template<typename real>
inline real clampTo( real value, real minimum, real maximum )
{
	return ( value < minimum ? minimum : ( value > maximum ? maximum : value ) );
}

//! Clamps a value if it is below a minimum.
template<typename real>
inline real clampAbove( real value, real minimum )
{
	return ( value < minimum ? minimum : value );
}

//! Clamps a value if it is above a maximum.
template<typename real>
inline real clampBelow( real value, real maximum )
{
	return ( value > maximum ? maximum : value );
}

//! Returns 1 if the value is positive or -1 if the value is negative.
template<typename real>
inline real sign( real value )
{
	return ( value < 0 ? -1 : 1 );
}

//! Returns the sign bit of the value: 0 if positive and 1 if negative.
template<typename real>
inline unsigned int signBit( real value )
{
	return ( value < 0 ) ? 1 : 0;
}

//! Returns the square of a number (the number multiplied by itself).
template<typename real>
inline real square( real value )
{
	return ( value * value );
}

//! Returns the square of a number multiplied by its sign.
//! This function keeps the square of negative numbers negative.
template<typename real>
inline real signedSquare( real v )
{
	return ( v < 0 ? -v * v : v * v );
}

//! Returns the cube of a number.
template<typename real>
inline real cube( real value )
{
	return ( value * value * value );
}

//! Computes the power-of-two immediately above a value.
inline uint32 nextPowerOf2( uint32 value )
{
	value |= ( value >> 1 );
	value |= ( value >> 2 );
	value |= ( value >> 4 );
	value |= ( value >> 8 );
	value |= ( value >> 16 );
	return ( value + 1 );
}

typedef Math<float> Mathf;
typedef Math<double> Mathd;

} // namespace vr

#endif
