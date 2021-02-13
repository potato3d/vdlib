#include <vdlib/Distance.h>
#include <vdlib/Plane.h>
#include <vdlib/Box.h>

using namespace vdlib;

float Distance::between( const vr::vec3f& point, const Plane& plane )
{
	return point.dot( plane.normal ) + plane.position;
}

float Distance::between( const vr::vec3f& point, const Box& box )
{
	// Work in the box's coordinate system
	const vr::vec3f& pointMinusCenter = point - box.center;

	// Compute squared distance
	float sqrDistance = 0.0f;
	float delta;

	for( unsigned int i = 0; i < 3; ++i )
	{
		float axisProjection = pointMinusCenter.dot( box.axis[i] );
		float currentExtents = box.extents[i];

		if( axisProjection < -currentExtents )
		{
			delta = axisProjection + currentExtents;
			sqrDistance += delta * delta;
		}
		else if( axisProjection > currentExtents )
		{
			delta = axisProjection - currentExtents;
			sqrDistance += delta * delta;
		}
	}

	return sqrtf( sqrDistance );
}
