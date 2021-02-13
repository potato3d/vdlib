/**
*	Algorithms to compute euclidean distance between geometric entities.
*	author: Paulo Ivson <psantos@tecgraf.puc-rio.br>
*	date:   5-Mar-2008
*/
#ifndef _VDLIB_DISTANCE_H_
#define _VDLIB_DISTANCE_H_

#include <vdlib/Common.h>

namespace vdlib {

class Distance
{
public:
	// Plane must be in Hessian Normal Form
	static float between( const vr::vec3f& point, const Plane& plane );

	// Precise distance from point to box
	static float between( const vr::vec3f& point, const Box& box );
};

} // namespace vdlib

#endif // _VDLIB_DISTANCE_H_
