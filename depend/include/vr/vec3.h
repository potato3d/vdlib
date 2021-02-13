#ifndef _VR_VEC3_H_
#define _VR_VEC3_H_

#include <vr/common.h>
#include <vr/math.h>

namespace vr {

template<typename real>
class vec3
{
public:
	// Public members
	union
	{
		struct { real x, y, z; };	//!< Treat the vector as a position or direction.
		struct { real r, g, b; };	//!< Treat the vector as a color.
		struct { real s, t, p; };	//!< Treat the vector as a parametric coordinate.
		real ptr[3];				//!< Treat the vector as an array.
	};

	// Constants
	inline static vec3<real> UNIT_X();  // ( 1, 0, 0 )
	inline static vec3<real> UNIT_Y();  // ( 0, 1, 0 )
	inline static vec3<real> UNIT_Z();  // ( 0, 0, 1 )

	// Construction
	inline vec3(); // Uninitialized
	inline vec3( real scalar1, real scalar2, real scalar3 );
	inline explicit vec3( real const * const tuple );

	// Getters
	inline real  operator[]( int32 i ) const;
	inline real& operator[]( int32 i );

	// Setters
	inline void set( real scalar1, real scalar2, real scalar3 );
	inline void set( real const * const tuple );

	// Arithmetic operations
	inline vec3<real> operator+( const vec3<real>& other ) const;
	inline vec3<real> operator-( const vec3<real>& other ) const;
	inline vec3<real> operator*( const vec3<real>& other ) const; // Componentwise
	inline vec3<real> operator*( real scalar ) const;
	inline vec3<real> operator-() const;

	// Arithmetic updates
	inline vec3<real>& operator+=( const vec3<real>& other );
	inline vec3<real>& operator-=( const vec3<real>& other );
	inline vec3<real>& operator*=( const vec3<real>& other ); // Componentwise
	inline vec3<real>& operator*=( real scalar );

	// Equality test (with no tolerance)
	inline bool operator==( const vec3<real>& other ) const;

	// Inequality test (with no tolerance)
	inline bool operator!=( const vec3<real>& other ) const;

	// Vector operations
	inline real length() const;
	inline real length2() const;

	// Used when previous length is needed
	inline real normalize();

	// Only normalizes if needed, saves unnecessary sqrt
	inline void tryNormalize();

	inline real dot( const vec3<real>& other ) const;
	inline vec3<real> cross( const vec3<real>& other ) const;

	// Normalize this first!
	inline void orthonormalBasis( vec3<real>& u, vec3<real>& v );
};

// Constants
template<typename real>
vec3<real> vec3<real>::UNIT_X()
{
	return vec3<real>( 1, 0, 0 );
}

template<typename real>
vec3<real> vec3<real>::UNIT_Y()
{
	return vec3<real>( 0, 1, 0 );
}

template<typename real>
vec3<real> vec3<real>::UNIT_Z()
{
	return vec3<real>( 0, 0, 1 );
}

// Construction
template<typename real>
vec3<real>::vec3()
{
	// Do not initialize for performance!
}

template<typename real>
vec3<real>::vec3( real scalar1, real scalar2, real scalar3 )
	: x( scalar1 ), y( scalar2 ), z( scalar3 )
{
}

template<typename real>
vec3<real>::vec3( real const * const tuple )
	: x( tuple[0] ), y( tuple[1] ), z( tuple[2] )
{
}

template<typename real>
real vec3<real>::operator[]( int32 i ) const
{
	return ptr[i];
}

template<typename real>
real& vec3<real>::operator[]( int32 i )
{
	return ptr[i];
}

// Setters
template<typename real>
void vec3<real>::set( real scalar1, real scalar2, real scalar3 )
{
	x = scalar1;
	y = scalar2;
	z = scalar3;
}

template<typename real>
void vec3<real>::set( real const * const tuple )
{
	x = tuple[0];
	y = tuple[1];
	z = tuple[2];
}

// Arithmetic operations
template<typename real>
vec3<real> vec3<real>::operator+( const vec3<real>& other ) const
{
	return vec3<real>( x + other.x, y + other.y, z + other.z );
}

template<typename real>
vec3<real> vec3<real>::operator-( const vec3<real>& other ) const
{
	return vec3<real>( x - other.x, y - other.y, z - other.z );
}

template<typename real>
vec3<real> vec3<real>::operator*( const vec3<real>& other ) const
{
	return vec3<real>( x * other.x, y * other.y, z * other.z );
}

template<typename real>
vec3<real> vec3<real>::operator*( real scalar ) const
{
	return vec3<real>( x * scalar, y * scalar, z * scalar );
}

template<typename real>
vec3<real> vec3<real>::operator-() const
{
	return vec3<real>( -x, -y, -z );
}

// Arithmetic updates
template<typename real>
vec3<real>& vec3<real>::operator+=( const vec3<real>& other )
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

template<typename real>
vec3<real>& vec3<real>::operator-=( const vec3<real>& other )
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

template<typename real>
vec3<real>& vec3<real>::operator*=( const vec3<real>& other )
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
	return *this;
}

template<typename real>
vec3<real>& vec3<real>::operator*=( real scalar )
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

// Equality test (with no tolerance)
template<typename real>
bool vec3<real>::operator==( const vec3<real>& other ) const
{
	return x == other.x && y == other.y && z == other.z;
}

// Inequality test (with no tolerance)
template<typename real>
bool vec3<real>::operator!=( const vec3<real>& other ) const
{
	return x != other.x || y != other.y || z != other.z;
}

// Vector operations
template<typename real>
real vec3<real>::length() const
{
	return sqrt( length2() );
}

template<typename real>
real vec3<real>::length2() const
{
	return x*x + y*y + z*z;
}

template<typename real>
real vec3<real>::normalize()
{
	real len = length();
	if( len > Math<real>::ZERO_TOLERANCE )
	{
		real invLen = static_cast<real>(1.0) / len;
		x *= invLen;
		y *= invLen;
		z *= invLen;
		return len;
	}
	else
	{
		x = 0;
		y = 0;
		z = 0;
		return 0;
	}
}

template<typename real>
void vec3<real>::tryNormalize()
{
	real len2 = length2();
	if( !vr::isEqual<real>( len2, 1 ) )
	{
		real invLen = static_cast<real>(1.0) / sqrt( len2 );
		x *= invLen;
		y *= invLen;
		z *= invLen;
	}
}

template<typename real>
real vec3<real>::dot( const vec3<real>& other ) const
{
	return x*other.x + y*other.y + z*other.z;
}

template<typename real>
vec3<real> vec3<real>::cross( const vec3<real>& other ) const
{
	return vec3<real>(	y*other.z - z*other.y,
					    z*other.x - x*other.z,
					    x*other.y - y*other.x );
}

// Normalize this first!
template<typename real>
void vec3<real>::orthonormalBasis( vec3<real>& u, vec3<real>& v )
{
	if( abs( x ) >= abs( y ) )
	{
		// this.x or this.z is the largest magnitude component, swap them
		const real invLength = 1.0f / sqrt( x*x + z*z );
		u[0] = -z*invLength;
		u[1] = (real)0.0;
		u[2] = +x*invLength;
		v[0] = y*u[2];
		v[1] = z*u[0] - x*u[2];
		v[2] = -y*u[0];
	}
	else
	{
		// this.y or this.z is the largest magnitude component, swap them
		const real invLength = 1.0f / sqrt( y*y + z*z );
		u[0] = (real)0.0;
		u[1] = +z*invLength;
		u[2] = -y*invLength;
		v[0] = y*u[2] - z*u[1];
		v[1] = -x*u[2];
		v[2] = x*u[1];
	}
}

typedef vec3<float>  vec3f;
typedef vec3<double> vec3d;

} // namespace vr

#endif
