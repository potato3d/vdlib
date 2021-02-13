#include <vdlib/Plane.h>

using namespace vdlib;

void Plane::set( float a, float b, float c, float d )
{
	normal.set( a, b, c );
	position = d;
}

void Plane::set( const vr::vec3f& _normal, const vr::vec3f& pointOnPlane )
{
	normal = _normal;
	position = -( _normal.dot( pointOnPlane ) );
}

void Plane::normalize()
{
	position /= normal.normalize();
}
