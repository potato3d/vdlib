#ifndef _VR_VEC2_H_
#define _VR_VEC2_H_

#include <vr/common.h>
#include <vr/math.h>

namespace vr {

template<typename real>
class vec2
{
public:
	// Public members
	union
	{
		struct { real x, y; };	//!< Treat the vector as a position or direction.
		struct { real s, t; };	//!< Treat the vector as a parametric coordinate.
		real ptr[2];			//!< Treat the vector as an array.
	};

	// Constants
	inline static vec2<real> UNIT_X();  // ( 1, 0 )
	inline static vec2<real> UNIT_Y();  // ( 0, 1 )

	// Construction
	inline vec2(); // Uninitialized
	inline vec2( real scalar1, real scalar2 );
	inline explicit vec2( real const * const pair );

	// Getters
	inline real  operator[]( int32 i ) const;
	inline real& operator[]( int32 i );

	// Setters
	inline void set( real scalar1, real scalar2 );
	inline void set( real const * const pair );

	// Arithmetic operations
	inline vec2<real> operator+( const vec2<real>& other ) const;
	inline vec2<real> operator-( const vec2<real>& other ) const;
	inline vec2<real> operator*( const vec2<real>& other ) const; // Componentwise
	inline vec2<real> operator*( real scalar ) const;
	inline vec2<real> operator-() const;

	// Arithmetic updates
	inline vec2<real>& operator+=( const vec2<real>& other );
	inline vec2<real>& operator-=( const vec2<real>& other );
	inline vec2<real>& operator*=( const vec2<real>& other ); // Componentwise
	inline vec2<real>& operator*=( real scalar );

	// Vector operations
	inline real length() const;
	inline real length2() const;

	// Used when previous length is needed
	inline real normalize();

	// Only normalizes if needed, saves unnecessary sqrt
	inline void tryNormalize();

	inline real dot( const vec2<real>& other ) const;
};

// Constants
template<typename real>
vec2<real> vec2<real>::UNIT_X()
{
	return vec2<real>( 1, 0, 0 );
}

template<typename real>
vec2<real> vec2<real>::UNIT_Y()
{
	return vec2<real>( 0, 1, 0 );
}

// Construction
template<typename real>
vec2<real>::vec2()
{
	// Do not initialize for performance!
}

template<typename real>
vec2<real>::vec2( real scalar1, real scalar2 )
	: x( scalar1 ), y( scalar2 )
{
}

template<typename real>
vec2<real>::vec2( real const * const pair )
	: x( pair[0] ), y( pair[1] )
{
}

template<typename real>
real vec2<real>::operator[]( int32 i ) const
{
	return ptr[i];
}

template<typename real>
real& vec2<real>::operator[]( int32 i )
{
	return ptr[i];
}

// Setters
template<typename real>
void vec2<real>::set( real scalar1, real scalar2 )
{
	x = scalar1;
	y = scalar2;
}

template<typename real>
void vec2<real>::set( real const * const pair )
{
	x = pair[0];
	y = pair[1];
}

// Arithmetic operations
template<typename real>
vec2<real> vec2<real>::operator+( const vec2<real>& other ) const
{
	return vec2<real>( x + other.x, y + other.y );
}

template<typename real>
vec2<real> vec2<real>::operator-( const vec2<real>& other ) const
{
	return vec2<real>( x - other.x, y - other.y );
}

template<typename real>
vec2<real> vec2<real>::operator*( const vec2<real>& other ) const
{
	return vec2<real>( x * other.x, y * other.y );
}

template<typename real>
vec2<real> vec2<real>::operator*( real scalar ) const
{
	return vec2<real>( x * scalar, y * scalar );
}

template<typename real>
vec2<real> vec2<real>::operator-() const
{
	return vec2<real>( -x, -y );
}

// Arithmetic updates
template<typename real>
vec2<real>& vec2<real>::operator+=( const vec2<real>& other )
{
	x += other.x;
	y += other.y;
	return *this;
}

template<typename real>
vec2<real>& vec2<real>::operator-=( const vec2<real>& other )
{
	x -= other.x;
	y -= other.y;
	return *this;
}

template<typename real>
vec2<real>& vec2<real>::operator*=( const vec2<real>& other )
{
	x *= other.x;
	y *= other.y;
	return *this;
}

template<typename real>
vec2<real>& vec2<real>::operator*=( real scalar )
{
	x *= scalar;
	y *= scalar;
	return *this;
}

// Vector operations
template<typename real>
real vec2<real>::length() const
{
	return sqrt( length2() );
}

template<typename real>
real vec2<real>::length2() const
{
	return x*x + y*y;
}

template<typename real>
real vec2<real>::normalize()
{
	real len = length();
	if( len > Math<real>::ZERO_TOLERANCE )
	{
		real invLen = static_cast<real>(1.0) / len;
		x *= invLen;
		y *= invLen;
		return len;
	}
	else
	{
		x = 0;
		y = 0;
		return 0;
	}
}

template<typename real>
void vec2<real>::tryNormalize()
{
	real len2 = length2();
	if( !vr::isEqual<real>( len2, 1 ) )
	{
		real invLen = static_cast<real>(1.0) / sqrt( len2 );
		x *= invLen;
		y *= invLen;
	}
}

template<typename real>
real vec2<real>::dot( const vec2<real>& other ) const
{
	return x*other.x + y*other.y;
}

typedef vec2<float>  vec2f;
typedef vec2<double> vec2d;

} // namespace vr

#endif
