#include <vdlib/Box.h>

using namespace vdlib;

void Box::computeVertices( vr::vec3f* vertices ) const
{
	vr::vec3f extendedAxis[3] =
	{
		axis[0] * extents[0],
		axis[1] * extents[1],
		axis[2] * extents[2]
	};

	vertices[0] = center - extendedAxis[0] - extendedAxis[1] - extendedAxis[2];
	vertices[1] = center + extendedAxis[0] - extendedAxis[1] - extendedAxis[2];
	vertices[2] = center + extendedAxis[0] + extendedAxis[1] - extendedAxis[2];
	vertices[3] = center - extendedAxis[0] + extendedAxis[1] - extendedAxis[2];
	vertices[4] = center - extendedAxis[0] - extendedAxis[1] + extendedAxis[2];
	vertices[5] = center + extendedAxis[0] - extendedAxis[1] + extendedAxis[2];
	vertices[6] = center + extendedAxis[0] + extendedAxis[1] + extendedAxis[2];
	vertices[7] = center - extendedAxis[0] + extendedAxis[1] + extendedAxis[2];
}

const vr::vec3f& Box::getLongestAxis() const
{
	if( extents[0] > extents[1] && extents[0] > extents[2] )
		return axis[0];
	else if( extents[1] > extents[2] )
		return axis[1];
	else
		return axis[2];
}
