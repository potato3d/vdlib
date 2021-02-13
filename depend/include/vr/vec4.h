#ifndef _VR_VEC4_H_
#define _VR_VEC4_H_

#include <vr/common.h>
#include <vr/math.h>

namespace vr {

template<typename real>
class vec4
{
public:
	// Public members
	union
	{
		struct { real x, y, z, w; };	//!< Treat the vector as a position or direction.
		struct { real r, g, b, a; };	//!< Treat the vector as a color.
		struct { real s, t, p, q; };	//!< Treat the vector as a parametric coordinate.
		real ptr[4];					//!< Treat the vector as an array.
	};

	// Constants
	inline static vec4<real> RED();   // ( 1, 0, 0, 1 )
	inline static vec4<real> GREEN(); // ( 0, 1, 0, 1 )
	inline static vec4<real> BLUE();  // ( 0, 0, 1, 1 )

	// Construction
	inline vec4(); // Uninitialized
	inline vec4( real scalar1, real scalar2, real scalar3, real scalar4 );
	inline explicit vec4( real const * const quad );

	// Getters
	inline real  operator[]( int32 i ) const;
	inline real& operator[]( int32 i );

	// Setters
	inline void set( real scalar1, real scalar2, real scalar3, real scalar4 );
	inline void set( real const * const quad );

	// Arithmetic operations
	inline vec4<real> operator+( const vec4<real>& other ) const;
	inline vec4<real> operator-( const vec4<real>& other ) const;
	inline vec4<real> operator*( const vec4<real>& other ) const; // Componentwise
	inline vec4<real> operator*( real scalar ) const;
	inline vec4<real> operator-() const;

	// Arithmetic updates
	inline vec4<real>& operator+=( const vec4<real>& other );
	inline vec4<real>& operator-=( const vec4<real>& other );
	inline vec4<real>& operator*=( const vec4<real>& other ); // Componentwise
	inline vec4<real>& operator*=( real scalar );

	// Vector operations
	inline real length() const;
	inline real length2() const;

	// Used when previous length is needed
	inline real normalize();

	// Only normalizes if needed, saves unnecessary sqrt
	inline void tryNormalize();

	inline real dot( const vec4<real>& other ) const;
};

// Constants
template<typename real>
vec4<real> vec4<real>::RED()
{
	return vec4<real>( 1, 0, 0, 1 );
}

template<typename real>
vec4<real> vec4<real>::GREEN()
{
	return vec4<real>( 0, 1, 0, 1 );
}

template<typename real>
vec4<real> vec4<real>::BLUE()
{
	return vec4<real>( 0, 0, 1, 1 );
}

// Construction
template<typename real>
vec4<real>::vec4()
{
	// Do not initialize for performance!
}

template<typename real>
vec4<real>::vec4( real scalar1, real scalar2, real scalar3, real scalar4 )
	: x( scalar1 ), y( scalar2 ), z( scalar3 ), w( scalar4 )
{
}

template<typename real>
vec4<real>::vec4( real const * const quad )
	: x( quad[0] ), y( quad[1] ), z( quad[2] ), w( quad[3] )
{
}

template<typename real>
real vec4<real>::operator[]( int32 i ) const
{
	return ptr[i];
}

template<typename real>
real& vec4<real>::operator[]( int32 i )
{
	return ptr[i];
}

// Setters
template<typename real>
void vec4<real>::set( real scalar1, real scalar2, real scalar3, real scalar4 )
{
	x = scalar1;
	y = scalar2;
	z = scalar3;
	w = scalar4;
}

template<typename real>
void vec4<real>::set( real const * const quad )
{
	x = quad[0];
	y = quad[1];
	z = quad[2];
	w = quad[3];
}

// Arithmetic operations
template<typename real>
vec4<real> vec4<real>::operator+( const vec4<real>& other ) const
{
	return vec4<real>( x + other.x, y + other.y, z + other.z, w + other.w );
}

template<typename real>
vec4<real> vec4<real>::operator-( const vec4<real>& other ) const
{
	return vec4<real>( x - other.x, y - other.y, z - other.z, w - other.w );
}

template<typename real>
vec4<real> vec4<real>::operator*( const vec4<real>& other ) const
{
	return vec4<real>( x * other.x, y * other.y, z * other.z, w * other.w );
}

template<typename real>
vec4<real> vec4<real>::operator*( real scalar ) const
{
	return vec4<real>( x * scalar, y * scalar, z * scalar, w * scalar );
}

template<typename real>
vec4<real> vec4<real>::operator-() const
{
	return vec4<real>( -x, -y, -z, -w );
}

// Arithmetic updates
template<typename real>
vec4<real>& vec4<real>::operator+=( const vec4<real>& other )
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

template<typename real>
vec4<real>& vec4<real>::operator-=( const vec4<real>& other )
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

template<typename real>
vec4<real>& vec4<real>::operator*=( const vec4<real>& other )
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
	w *= other.w;
	return *this;
}

template<typename real>
vec4<real>& vec4<real>::operator*=( real scalar )
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}

// Vector operations
template<typename real>
real vec4<real>::length() const
{
	return sqrt( length2() );
}

template<typename real>
real vec4<real>::length2() const
{
	return x*x + y*y + z*z + w*w;
}

template<typename real>
real vec4<real>::normalize()
{
	real len = length();
	if( len > Math<real>::ZERO_TOLERANCE() )
	{
		real invLen = static_cast<real>(1.0) / len;
		x *= invLen;
		y *= invLen;
		z *= invLen;
		w *= invLen;
		return len;
	}
	else
	{
		x = 0;
		y = 0;
		z = 0;
		w = 0;
		return 0;
	}
}

template<typename real>
void vec4<real>::tryNormalize()
{
	real len2 = length2();
	if( !vr::isEqual( len2, 1 ) )
	{
		real invLen = static_cast<real>(1.0) / sqrt( len2 );
		x *= invLen;
		y *= invLen;
		z *= invLen;
		w *= invLen;
	}
}

template<typename real>
real vec4<real>::dot( const vec4<real>& other ) const
{
	return x*other.x + y*other.y + z*other.z + w*other.w;
}

typedef vec4<float>  vec4f;
typedef vec4<double> vec4d;

}  // namespace vr

#endif
