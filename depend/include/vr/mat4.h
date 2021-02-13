#ifndef _VR_MAT4_H_
#define _VR_MAT4_H_

#include <vr/common.h>
#include <vr/vec3.h>
#include <vr/quat.h>

namespace vr {

/*!
	\brief 4x4 Homogeneous Transformation Matrix.
	This is a template class. You should use the classes mat4f (for single-precision)
	or mat4d (for double-precision) instead.
 */
template<typename real>
class mat4
{
public:
	// Construction
	inline mat4(); // Uninitialized
	inline mat4( const mat4<real>& other );
	inline explicit mat4( real const * const ptr );
	inline mat4( real a00, real a01, real a02, real a03,
					real a10, real a11, real a12, real a13,
					real a20, real a21, real a22, real a23,
					real a30, real a31, real a32, real a33 );

	// Analysis
	inline bool isNaN() const;
	inline bool isIdentity() const;

	// Assignment
	inline mat4<real>& operator=( const mat4<real>& other );

	// Getters
	inline real& operator()( int32 row, int32 col );
	inline real  operator()( int32 row, int32 col ) const;
	inline const real* ptr() const;

	inline void getTranslation( vec3<real>& trans ) const;
	inline void getScale( vec3<real>& getScale ) const;
	inline void getRotation( quat<real>& q ) const;

	// Setters
	inline void set( const quat<real>& q_in );
	inline void set( const mat4<real>& other );
	inline void set( real const * const ptr );
	inline void set( real a00, real a01, real a02, real a03,
					 real a10, real a11, real a12, real a13,
					 real a20, real a21, real a22, real a23,
					 real a30, real a31, real a32, real a33 );

	// Only alter specific parts of this
	inline void setRow( int32 row, real scalar0, real scalar1, real scalar2, real scalar3 );
	inline void setTranslation( const vec3<real>& trans );
	inline void setTranslation( real x, real y, real z );

	// Makes this a specific kind of matrix
	inline void makeIdentity();

	inline void makeScale( const vec3<real>& scale );
	inline void makeScale( real x, real y, real z );

	inline void makeTranslation( const vec3<real>& trans );
	inline void makeTranslation( real x, real y, real z );

	inline void makeRotation( const vec3<real>& from, const vec3<real>& to );
	inline void makeRotation( real radians, const vec3<real>& axis );
	inline void makeRotation( real radians, real x, real y, real z );
	inline void makeRotation( real angle1, const vec3<real>& axis1, 
							  real angle2, const vec3<real>& axis2, 
							  real angle3, const vec3<real>& axis3 );

	// OpenGL Matrices

	// Set to an orthographic projection.
	inline void makeOrtho( real left, real right, real bottom, real top, real zNear, real zFar );

	// Get the orthographic settings of the orthographic projection matrix.
	// Note, if matrix is not an orthographic matrix then invalid values will be returned.
	inline bool getOrtho( real& left, real& right, real& bottom, real& top, real& zNear, real& zFar ) const;

	// Set to a 2D orthographic projection.
	inline void makeOrtho2D( real left, real right, real bottom, real top );

	// Set to a generic perspective projection.
	inline void makeFrustum( real left, real right, real bottom, real top, real zNear, real zFar );

	// Get the frustum settings of a perspective projection matrix.
	// Note, if matrix is not a perspective matrix then invalid values will be returned.
	inline bool getFrustum( real& left, real& right, real& bottom, real& top, real& zNear, real& zFar ) const;

	// Set to a symmetrical perspective projection.
	// Aspect ratio is defined as width/height.
	inline void makePerspective( real fovy, real aspectRatio, real zNear, real zFar );

	// Get the frustum settings of a symmetric perspective projection matrix.
	// Return false if matrix is not a perspective matrix, where parameter values are undefined. 
	// Note, if matrix is not a symmetric perspective matrix then the shear will be lost.
	// Asymmetric matrices occur when stereo, power walls, caves and reality center display are used.
	// In these configuration one should use the frustum methods instead.
	inline bool getPerspective( real& fovy, real& aspectRatio, real& zNear, real& zFar ) const;

	// Set the position and orientation to be a view matrix, using the same convention as gluLookAt.
	inline void makeLookAt( const vec3<real>& eye, const vec3<real>& center, const vec3<real>& up );

	// Get to the position and orientation of a modelview matrix, using the same convention as gluLookAt.
	inline void getLookAt( vec3<real>& eye, vec3<real>& center, vec3<real>& up, real lookDistance = 1.0f ) const;

	// Arithmetic operations
	inline mat4<real> operator*( real scalar ) const;
	inline mat4<real> operator-() const;

	// Arithmetic updates
	inline mat4<real>& operator*=( real scalar );

	// Matrix operations
	inline void transpose();
	inline void invert();

	// Assumes this is a rigid-body transformation.
	// Avoids expensive matrix inversion by transposing 3x3 sub-matrix and inverting the translation vector.
	inline void invertRBT();

	// Assumes this is a rotation matrix.
	// Ortho-normalize the 3x3 rotation and scale matrix.
	inline void orthoNormalize();

	// Apply this transformation to v
	inline void transform( vec3<real>& v, real w = 1 ) const;
	inline void transposedTransform( vec3<real>& v, real w = 1 ) const;

	// Apply only the 3x3 transformation to v
	inline void transform3x3( vec3<real>& v ) const;
	inline void transposedTransform3x3( vec3<real>& v ) const;

	// Sets this as the result of the product of a and b <this = a * b>
	inline void product( const mat4<real>& a, const mat4<real>& b );
	
private:
	real _m[4][4];
};

/************************************************************************/
/* 'Secret' Matrix Manipulation Functions                               */
/************************************************************************/
void _mat4_getRotation( const mat4<float>& r, quat<float>& q );
void _mat4_getRotation( const mat4<double>& r, quat<double>& q );
void _mat4_set( mat4<float>& r, const quat<float>& q_in );
void _mat4_set( mat4<double>& r, const quat<double>& q_in );
void _mat4_makeIdentity( vr::mat4<float>& r );
void _mat4_makeIdentity( vr::mat4<double>& r );
void _mat4_makeScale( vr::mat4<float>& r, const vr::vec3<float>& scale );
void _mat4_makeScale( vr::mat4<double>& r, const vr::vec3<double>& scale );
void _mat4_makeScale( vr::mat4<float>& r, float x, float y, float z );
void _mat4_makeScale( vr::mat4<double>& r, double x, double y, double z );
void _mat4_makeTranslation( vr::mat4<float>& r, const vr::vec3<float>& trans );
void _mat4_makeTranslation( vr::mat4<double>& r, const vr::vec3<double>& trans );
void _mat4_makeTranslation( vr::mat4<float>& r, float x, float y, float z );
void _mat4_makeTranslation( vr::mat4<double>& r, double x, double y, double z );
void _mat4_makeRotation( vr::mat4<float>& r, const vr::vec3<float>& from, const vr::vec3<float>& to );
void _mat4_makeRotation( vr::mat4<double>& r, const vr::vec3<double>& from, const vr::vec3<double>& to );
void _mat4_makeRotation( vr::mat4<float>& r, float radians, const vr::vec3<float>& axis );
void _mat4_makeRotation( vr::mat4<double>& r, double radians, const vr::vec3<double>& axis );
void _mat4_makeRotation( vr::mat4<float>& r, float angle1, const vr::vec3<float>& axis1,  
                         float angle2, const vr::vec3<float>& axis2,  
                         float angle3, const vr::vec3<float>& axis3 );
void _mat4_makeRotation( vr::mat4<double>& r, double angle1, const vr::vec3<double>& axis1,  
                         double angle2, const vr::vec3<double>& axis2,  
                         double angle3, const vr::vec3<double>& axis3 );
void _mat4_makeOrtho( vr::mat4<float>& r, float left, float right, float bottom, float top, float zNear, float zFar );
void _mat4_makeOrtho( vr::mat4<double>& r, double left, double right, double bottom, double top, double zNear, double zFar );
bool _mat4_getOrtho( const vr::mat4<float>& r, float& left, float& right, float& bottom, float& top, float& zNear, float& zFar );
bool _mat4_getOrtho( const vr::mat4<double>& r, double& left, double& right, double& bottom, double& top, double& zNear, double& zFar );
void _mat4_makeFrustum( vr::mat4<float>& r, float left, float right, float bottom, float top, float zNear, float zFar );
void _mat4_makeFrustum( vr::mat4<double>& r, double left, double right, double bottom, double top, double zNear, double zFar );
bool _mat4_getFrustum( const vr::mat4<float>& r, float& left, float& right, float& bottom, float& top, float& zNear, float& zFar );
bool _mat4_getFrustum( const vr::mat4<double>& r, double& left, double& right, double& bottom, double& top, double& zNear, double& zFar );
void _mat4_makePerspective( vr::mat4<float>& r, float fovy, float aspectRatio, float zNear, float zFar );
void _mat4_makePerspective( vr::mat4<double>& r, double fovy, double aspectRatio, double zNear, double zFar );
bool _mat4_getPerspective( const vr::mat4<float>& r, float& fovy, float& aspectRatio, float& zNear, float& zFar );
bool _mat4_getPerspective( const vr::mat4<double>& r, double& fovy, double& aspectRatio, double& zNear, double& zFar );
void _mat4_makeLookAt( vr::mat4<float>& r, const vr::vec3<float>& eye, const vr::vec3<float>& center, const vr::vec3<float>& up );
void _mat4_makeLookAt( vr::mat4<double>& r, const vr::vec3<double>& eye, const vr::vec3<double>& center, const vr::vec3<double>& up );
void _mat4_getLookAt( const vr::mat4<float>& r, vr::vec3<float>& eye, vr::vec3<float>& center, vr::vec3<float>& up, float lookDistance );
void _mat4_getLookAt( const vr::mat4<double>& r, vr::vec3<double>& eye, vr::vec3<double>& center, vr::vec3<double>& up, double lookDistance );
void _mat4_transpose( vr::mat4<float>& r );
void _mat4_transpose( vr::mat4<double>& r );
void _mat4_invert( vr::mat4<float>& r );
void _mat4_invert( vr::mat4<double>& r );
void _mat4_product( vr::mat4<float>& r, const vr::mat4<float>& a, const vr::mat4<float>& b );
void _mat4_product( vr::mat4<double>& r, const vr::mat4<double>& a, const vr::mat4<double>& b );

/************************************************************************/
/* mat4<real> Members                                                   */
/************************************************************************/

// Construction
template<typename real>
mat4<real>::mat4()
{
	// Do not initialize for performance!
}

template<typename real>
mat4<real>::mat4( const mat4<real>& other )
{
	set( other.ptr() );
}

template<typename real>
mat4<real>::mat4( real const * const ptr )
{
	set( ptr );
}

template<typename real>
mat4<real>::mat4( real a00, real a01, real a02, real a03,  
						  real a10, real a11, real a12, real a13,  
						  real a20, real a21, real a22, real a23,  
						  real a30, real a31, real a32, real a33 )
{
	setRow( 0, a00, a01, a02, a03 );
	setRow( 1, a10, a11, a12, a13 );
	setRow( 2, a20, a21, a22, a23 );
	setRow( 3, a30, a31, a32, a33 );
}

// Analysis
template<typename real>
bool mat4<real>::isNaN() const
{
	return	vr::isNaN( _m[0][0] ) ||
			vr::isNaN( _m[0][1] ) ||
			vr::isNaN( _m[0][2] ) ||
			vr::isNaN( _m[0][3] ) ||
			vr::isNaN( _m[1][0] ) ||
			vr::isNaN( _m[1][1] ) ||
			vr::isNaN( _m[1][2] ) ||
			vr::isNaN( _m[1][3] ) ||
			vr::isNaN( _m[2][0] ) ||
			vr::isNaN( _m[2][1] ) ||
			vr::isNaN( _m[2][2] ) ||
			vr::isNaN( _m[2][3] ) ||
			vr::isNaN( _m[3][0] ) ||
			vr::isNaN( _m[3][1] ) ||
			vr::isNaN( _m[3][2] ) ||
			vr::isNaN( _m[3][3] );
}

template<typename real>
bool mat4<real>::isIdentity() const
{
	return ( _m[0][0] == 1 ) && ( _m[0][1] == 0 ) && ( _m[0][2] == 0 ) && ( _m[0][3] == 0 ) &&
		   ( _m[1][0] == 0 ) && ( _m[1][1] == 1 ) && ( _m[1][2] == 0 ) && ( _m[1][3] == 0 ) &&
		   ( _m[2][0] == 0 ) && ( _m[2][1] == 0 ) && ( _m[2][2] == 1 ) && ( _m[2][3] == 0 ) &&
		   ( _m[3][0] == 0 ) && ( _m[3][1] == 0 ) && ( _m[3][2] == 0 ) && ( _m[3][3] == 1 );
}

// Assignment
template<typename real>
mat4<real>& mat4<real>::operator=( const mat4<real>& other )
{
	if( &other == this )
		return *this;
	set( other.ptr() );
	return *this;
}

// Getters
template<typename real>
real& mat4<real>::operator()( int32 row, int32 col )
{
	return _m[row][col];
}

template<typename real>
real mat4<real>::operator()( int32 row, int32 col ) const
{
	return _m[row][col];
}

template<typename real>
const real* mat4<real>::ptr() const
{
	return reinterpret_cast<const real*>(_m);
}

template<typename real>
void mat4<real>::getTranslation( vec3<real>& trans ) const
{
	trans.set( _m[3] );
}

template<typename real>
void mat4<real>::getScale( vec3<real>& scale ) const
{
	scale.set( sqrt( _m[0][0]*_m[0][0] + _m[1][0]*_m[1][0] + _m[2][0]*_m[2][0] ),
			   sqrt( _m[0][1]*_m[0][1] + _m[1][1]*_m[1][1] + _m[2][1]*_m[2][1] ),
			   sqrt( _m[0][2]*_m[0][2] + _m[1][2]*_m[1][2] + _m[2][2]*_m[2][2] ) );
}

template<typename real>
void mat4<real>::getRotation( quat<real>& q ) const
{
	_mat4_getRotation( *this, q );
}

// Setters
template<typename real>
void mat4<real>::set( const quat<real>& q_in )
{
	_mat4_set( *this, q_in );
}

template<typename real>
void mat4<real>::set( const mat4<real>& other )
{
	set( other.ptr() );
}

template<typename real>
void mat4<real>::set( real const* const ptr )
{
	std::copy( ptr, ptr + 16, reinterpret_cast<real*>( _m ) );
}

template<typename real>
void mat4<real>::set( real a00, real a01, real a02, real a03,  
						     real a10, real a11, real a12, real a13,  
							 real a20, real a21, real a22, real a23,  
							 real a30, real a31, real a32, real a33 )
{
	setRow( 0, a00, a01, a02, a03 );
	setRow( 1, a10, a11, a12, a13 );
	setRow( 2, a20, a21, a22, a23 );
	setRow( 3, a30, a31, a32, a33 );
}

// Only alter specific parts of this
template<typename real>
void mat4<real>::setRow( int32 row, real scalar0, real scalar1, real scalar2, real scalar3 )
{
	_m[row][0] = scalar0;
	_m[row][1] = scalar1;
	_m[row][2] = scalar2;
	_m[row][3] = scalar3;
}

template<typename real>
void mat4<real>::setTranslation( const vec3<real>& trans )
{
	setTranslation( trans.x, trans.y, trans.z );
}

template<typename real>
void mat4<real>::setTranslation( real x, real y, real z )
{
	_m[3][0] = x;
	_m[3][1] = y;
	_m[3][2] = z;
}

// Makes this a specific kind of matrix
template<typename real>
void mat4<real>::makeIdentity()
{
	_mat4_makeIdentity( *this );
}

template<typename real>
void mat4<real>::makeScale( const vec3<real>& scale )
{
	_mat4_makeScale( *this, scale );
}

template<typename real>
void mat4<real>::makeScale( real x, real y, real z )
{
	_mat4_makeScale( *this, x, y, z );
}

template<typename real>
void mat4<real>::makeTranslation( const vec3<real>& trans )
{
	_mat4_makeTranslation( *this, trans );
}

template<typename real>
void mat4<real>::makeTranslation( real x, real y, real z )
{
	_mat4_makeTranslation( *this, x, y, z );
}

template<typename real>
void mat4<real>::makeRotation( const vec3<real>& from, const vec3<real>& to )
{
	_mat4_makeRotation( *this, from, to );
}

template<typename real>
void mat4<real>::makeRotation( real radians, const vec3<real>& axis )
{
	_mat4_makeRotation( *this, radians, axis );
}

template<typename real>
void mat4<real>::makeRotation( real radians, real x, real y, real z )
{
	makeRotation( radians, vec3<real>( x, y, z ) );
}

template<typename real>
void mat4<real>::makeRotation( real angle1, const vec3<real>& axis1,  
									 real angle2, const vec3<real>& axis2,  
									 real angle3, const vec3<real>& axis3 )
{
	_mat4_makeRotation( *this, angle1, axis1, angle2, axis2, angle3, axis3 );
}

// OpenGL Matrices
template<typename real>
void mat4<real>::makeOrtho( real left, real right, real bottom, real top, real zNear, real zFar )
{
	_mat4_makeOrtho( *this, left, right, bottom, top, zNear, zFar );
}

template<typename real>
bool mat4<real>::getOrtho( real& left, real& right, real& bottom, real& top, real& zNear, real& zFar ) const
{
	return _mat4_getOrtho( *this, left, right, bottom, top, zNear, zFar );
}

template<typename real>
void mat4<real>::makeOrtho2D( real left, real right, real bottom, real top )
{
	void _vr_mat4_makeOrtho2D( vr::mat4<real>& r, real left, real right, real bottom, real top );
	_vr_mat4_makeOrtho2D( *this, left, right, bottom, top );
}

template<typename real>
void mat4<real>::makeFrustum( real left, real right, real bottom, real top, real zNear, real zFar )
{
	_mat4_makeFrustum( *this, left, right, bottom, top, zNear, zFar );
}

template<typename real>
bool mat4<real>::getFrustum( real& left, real& right, real& bottom, real& top, real& zNear, real& zFar ) const
{
	return _mat4_getFrustum( *this, left, right, bottom, top, zNear, zFar );
}

template<typename real>
void mat4<real>::makePerspective( real fovy, real aspectRatio, real zNear, real zFar )
{
	_mat4_makePerspective( *this, fovy, aspectRatio, zNear, zFar );
}

template<typename real>
bool mat4<real>::getPerspective( real& fovy, real& aspectRatio, real& zNear, real& zFar ) const
{
	return _mat4_getPerspective( *this, fovy, aspectRatio, zNear, zFar );
}

template<typename real>
void mat4<real>::makeLookAt( const vec3<real>& eye, const vec3<real>& center, const vec3<real>& up )
{
	_mat4_makeLookAt( *this, eye, center, up );
}

template<typename real>
void mat4<real>::getLookAt( vec3<real>& eye, vec3<real>& center, vec3<real>& up, real lookDistance ) const
{
	_mat4_getLookAt( *this, eye, center, up, lookDistance );
}

// Matrix operations
template<typename real>
void mat4<real>::transpose()
{
	_mat4_transpose( *this );
}

template<typename real>
void mat4<real>::invert()
{
	_mat4_invert( *this );
}

template<typename real>
void mat4<real>::product( const mat4<real>& a, const mat4<real>& b )
{
	_mat4_product( *this, a, b );
}

// Arithmetic operations
template<typename real>
mat4<real> mat4<real>::operator*( real scalar ) const
{
	return mat4<real>( _m[0][0]*scalar, _m[0][1]*scalar, _m[0][2]*scalar, _m[0][3]*scalar,
						  _m[1][0]*scalar, _m[1][1]*scalar, _m[1][2]*scalar, _m[1][3]*scalar,
						  _m[2][0]*scalar, _m[2][1]*scalar, _m[2][2]*scalar, _m[2][3]*scalar,
						  _m[3][0]*scalar, _m[3][1]*scalar, _m[3][2]*scalar, _m[3][3]*scalar );
}

template<typename real>
mat4<real> mat4<real>::operator-() const
{
	return mat4<real>( -_m[0][0], -_m[0][1], -_m[0][2], -_m[0][3],
						  -_m[1][0], -_m[1][1], -_m[1][2], -_m[1][3],
						  -_m[2][0], -_m[2][1], -_m[2][2], -_m[2][3],
						  -_m[3][0], -_m[3][1], -_m[3][2], -_m[3][3] );
}

template<typename real>
mat4<real>& mat4<real>::operator*=( real scalar )
{
	_m[0][0] *= scalar;
	_m[0][1] *= scalar;
	_m[0][2] *= scalar;
	_m[0][3] *= scalar;
	_m[1][0] *= scalar;
	_m[1][1] *= scalar;
	_m[1][2] *= scalar;
	_m[1][3] *= scalar;
	_m[2][0] *= scalar;
	_m[2][1] *= scalar;
	_m[2][2] *= scalar;
	_m[2][3] *= scalar;
	_m[3][0] *= scalar;
	_m[3][1] *= scalar;
	_m[3][2] *= scalar;
	_m[3][3] *= scalar;
	return *this;
}

// Matrix operations
template<typename real>
void mat4<real>::invertRBT()
{
	// negate the translation: 1) transform it by the 3x3 sub-matrix; 2) negate it;
	real tx = _m[3][0];
	real ty = _m[3][1];
	real tz = _m[3][2];
	_m[3][0] = -( _m[0][0]*tx + _m[0][1]*ty + _m[0][2]*tz );
	_m[3][1] = -( _m[1][0]*tx + _m[1][1]*ty + _m[1][2]*tz );
	_m[3][2] = -( _m[2][0]*tx + _m[2][1]*ty + _m[2][2]*tz );

	// now, simply transpose the 3x3 sub-matrix
	real temp = _m[1][0];
	_m[1][0] = _m[0][1];
	_m[0][1] = temp;

	temp = _m[2][0];
	_m[2][0] = _m[0][2];
	_m[0][2] = temp;

	temp = _m[2][1];
	_m[2][1] = _m[1][2];
	_m[1][2] = temp;
}

template<typename real>
void mat4<real>::orthoNormalize()
{
	real x_colMag = _m[0][0]*_m[0][0] + _m[1][0]*_m[1][0] + _m[2][0]*_m[2][0];
	real y_colMag = _m[0][1]*_m[0][1] + _m[1][1]*_m[1][1] + _m[2][1]*_m[2][1];
	real z_colMag = _m[0][2]*_m[0][2] + _m[1][2]*_m[1][2] + _m[2][2]*_m[2][2];

	if( !vr::isEqual<real>( x_colMag, 1 ) && !vr::isEqual<real>( x_colMag, 0 ) )
	{
		x_colMag = static_cast<real>(1.0) / sqrt( x_colMag );
		_m[0][0] *= x_colMag;
		_m[1][0] *= x_colMag;
		_m[2][0] *= x_colMag;
	}

	if( !vr::isEqual<real>( y_colMag, 1 ) && !vr::isEqual<real>( y_colMag, 0 ) )
	{
		y_colMag = static_cast<real>(1.0) / sqrt( y_colMag );
		_m[0][1] *= y_colMag;
		_m[1][1] *= y_colMag;
		_m[2][1] *= y_colMag;
	}

	if( !vr::isEqual<real>( z_colMag, 1 ) && !vr::isEqual<real>( z_colMag, 0 ) )
	{
		z_colMag = static_cast<real>(1.0) / sqrt( z_colMag );
		_m[0][2] *= z_colMag;
		_m[1][2] *= z_colMag;
		_m[2][2] *= z_colMag;
	}
}

template<typename real>
void mat4<real>::transform( vec3<real>& v, real w ) const
{
	const real vx = v.x;
	const real vy = v.y;
	const real vz = v.z;
	const real d = 1.0f / ( _m[0][3]*vx + _m[1][3]*vy + _m[2][3]*vz + _m[3][3]*w ) ;

	v.x = ( _m[0][0]*vx + _m[1][0]*vy + _m[2][0]*vz + _m[3][0]*w ) * d;
	v.y = ( _m[0][1]*vx + _m[1][1]*vy + _m[2][1]*vz + _m[3][1]*w ) * d;
	v.z = ( _m[0][2]*vx + _m[1][2]*vy + _m[2][2]*vz + _m[3][2]*w ) * d;
}

template<typename real>
void mat4<real>::transposedTransform( vec3<real>& v, real w ) const
{
	const real vx = v.x;
	const real vy = v.y;
	const real vz = v.z;
	const real d = 1.0f / ( _m[3][0]*vx + _m[3][1]*vy + _m[3][2]*vz + _m[3][3]*w ) ;

	v.x = ( _m[0][0]*v.x + _m[0][1]*v.y + _m[0][2]*v.z + _m[0][3]*w ) * d;
	v.y = ( _m[1][0]*v.x + _m[1][1]*v.y + _m[1][2]*v.z + _m[1][3]*w ) * d;
	v.z = ( _m[2][0]*v.x + _m[2][1]*v.y + _m[2][2]*v.z + _m[2][3]*w ) * d;
}

template<typename real>
void mat4<real>::transform3x3( vec3<real>& v ) const
{
	const real vx = v.x;
	const real vy = v.y;
	const real vz = v.z;
	v.x = _m[0][0]*vx + _m[1][0]*vy + _m[2][0]*vz;
	v.y = _m[0][1]*vx + _m[1][1]*vy + _m[2][1]*vz;
	v.z = _m[0][2]*vx + _m[1][2]*vy + _m[2][2]*vz;
}

template<typename real>
void mat4<real>::transposedTransform3x3( vec3<real>& v ) const
{
	const real vx = v.x;
	const real vy = v.y;
	const real vz = v.z;
	v.x = _m[0][0]*vx + _m[0][1]*vy +_m[0][2]*vz;
	v.y = _m[1][0]*vx + _m[1][1]*vy +_m[1][2]*vz;
	v.z = _m[2][0]*vx + _m[2][1]*vy +_m[2][2]*vz;
}

/************************************************************************/
/* mat4<real> Instantiations                                        */
/************************************************************************/

/*
	mat4 class with a single-precision implementation.
 */
class VRBASE_EXPORT mat4f : public mat4<float>
{
public:
	inline mat4f() : mat4<float>() {;} // Uninitialized
	inline mat4f( const mat4<float>& other ) : mat4<float>( other ) {;}
	inline explicit mat4f( float const * const ptr ) : mat4<float>( ptr ) {;}
	inline mat4f( float a00, float a01, float a02, float a03,
					 float a10, float a11, float a12, float a13,
					 float a20, float a21, float a22, float a23,
					 float a30, float a31, float a32, float a33 ) 
		: mat4<float>( a00, a01, a02, a03,
						  a10, a11, a12, a13,
						  a20, a21, a22, a23,
						  a30, a31, a32, a33 ) {;}
};

/*
	mat4 class with a double-precision implementation.
 */
class VRBASE_EXPORT mat4d : public mat4<double>
{
public:
	inline mat4d() : mat4<double>() {;} // Uninitialized
	inline mat4d( const mat4<double>& other ) : mat4<double>( other ) {;}
	inline explicit mat4d( double const * const ptr ) : mat4<double>( ptr ) {;}
	inline mat4d( double a00, double a01, double a02, double a03,
					  double a10, double a11, double a12, double a13,
					  double a20, double a21, double a22, double a23,
					  double a30, double a31, double a32, double a33 ) 
		: mat4<double>( a00, a01, a02, a03,
						   a10, a11, a12, a13,
						   a20, a21, a22, a23,
						   a30, a31, a32, a33 ) {;}
};

} // namespace vr

#endif
