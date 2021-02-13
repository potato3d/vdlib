#include <vdlib/Intersection.h>
#include <vdlib/Plane.h>
#include <vdlib/Box.h>
#include <vdlib/Distance.h>

using namespace vdlib;

int Intersection::between( const Plane& plane, const Box& box )
{
	const float projectedCenter = Distance::between( box.center, plane );
	const float projectedRadius = vr::abs( plane.normal.dot( box.axis[0] ) * box.extents[0] ) +
								  vr::abs( plane.normal.dot( box.axis[1] ) * box.extents[1] ) +
								  vr::abs( plane.normal.dot( box.axis[2] ) * box.extents[2] );

	// Totally inside
	if( projectedCenter >= projectedRadius )
		return +1;

	// Totally outside
	if( projectedCenter <= ( -projectedRadius ) )
		return -1;

	// Intersected
	return 0;
}
