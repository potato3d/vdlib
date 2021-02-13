#ifndef _VR_INTERPOLATION_H_
#define _VR_INTERPOLATION_H_

#include <vr/vec2.h>
#include <vr/vec3.h>

/*!
	\file interpolation.h Contains basic functions to interpolate between values.
 */

namespace vr {

/*!
	Linear interpolation from <tt>a</tt> to <tt>b</tt> as <tt>t</tt> goes from 0 to 1.
 */
template<typename real>
inline real lerp( real a, real b, real t )
{
	return ( 1 - t ) * a + b * t;
}

/*!
	Linear interpolation from <tt>a</tt> to <tt>b</tt> as <tt>t</tt> goes from 0 to 1.
 */
template<typename T, typename real>
inline T lerp( T a, T b, real t )
{
	return a * ( 1 - t ) + b * t;
}

/*!
	Bilinear Interpolation.
	The <tt>values</tt> array should contain 4 elements and follow this convention:
	<pre>
             (1,1)
       [2]--[3] 
        |    |
       [0]--[1]
    (0,0)
	</pre>
	Parameter <tt>st</tt> goes from 0 to 1 in both dimensions.
 */
template<typename real>
inline real bilerp( const real values[4], const vec2<real>& st )
{
	vec2<real> stc( 1, 1 );
	stc -= st;

	real bottom = ( stc.s * values[0] ) + ( st.s * values[1] );
	real top = ( stc.s * values[2] ) + ( st.s * values[3] );
	return ( stc.t * bottom ) + ( st.t * top );
}

/*!
	Trilinear Interpolation.
	The <tt>values</tt> array should contain 4 elements and follow this convention:
	<pre>
        [2]-----[3]
         |       |
     [6]-----[7] |     y
      |  |    |  |     |
      | [0]---|-[1]    |____x
      |       |       /
     [4]-----[5]     /z
	</pre>
	Parameter <tt>stp</tt> goes from 0 to 1 in the three dimensions.
 */
template<typename real>
inline real trilerp( const real values[8], const vec3<real>& stp )
{
	real s = stp.s * 2 - 1;
	real t = stp.t * 2 - 1;
	real p = stp.p * 2 - 1;

	real s0 = 1 - s;
	real s1 = 1 + s;
	real t0 = 1 - t;
	real t1 = 1 + t;
	real p0 = 1 - p;
	real p1 = 1 + p;

	real w[8];
	w[0] = ( s0 * t0 * p0 );
	w[1] = ( s1 * t0 * p0 );
	w[2] = ( s0 * t1 * p0 );
	w[3] = ( s1 * t1 * p0 );
	w[4] = ( s0 * t0 * p1 );
	w[5] = ( s1 * t0 * p1 );
	w[6] = ( s0 * t1 * p1 );
	w[7] = ( s1 * t1 * p1 );

	real v = 0;
	for( int i = 0; i < 8; ++i )
		v += w[i] * values[i];

	return v * static_cast<real>( 1.0 / 8.0 );
}

/*!
	Spherical Linear Interpolation.
	Interpolates along an arc over a unit sphere, from <tt>v0</tt> to <tt>v1</tt>,
	as <tt>t</tt> goes from 0 to 1.	Both <tt>v0</tt> and <tt>v1</tt> must be unit vectors.
	In this case, the result is also an unit vector.
 */
template<typename real>
inline bool slerp( const vec3<real>& v0, const vec3<real>& v1, real t, vec3<real>& result )
{
	real theta = v0.dot( v1 );

	/*
		We must handle special cases when |v0 . v1| = 1.
		If theta is 1, both vectors are equal and the result is set to either one of them.
		If theta is -1, v0 and v1 are parallel and have opposite directions. In this case
		the rotation is ambiguous: because v0 and v1 are colinear, there are infinite
		geodesics connecting both points.
	 */

	if( isEqual<real>( theta, -1 ) )
		return false;

	if( isEqual<real>( theta, 1 ) )
	{
		result = v0;
	}
	else
	{
		// compute the actual angle between the vectors
		theta = acos( theta );

		// result = w0 * v0 + w1 * v1
		real invSinAngle = 1 / sin( theta );
		real w0 = sin( ( 1 - t ) * theta ) * invSinAngle;
		real w1 = sin( t * theta ) * invSinAngle;

		result = v0;
		result *= w0;
		result += v1 * w1;
	}

	return true;
}

} // namespace vr

#endif
