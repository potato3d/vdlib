/**
*	A generic oriented box, can be used as AABB or OBB.
*	author: Paulo Ivson <psantos@tecgraf.puc-rio.br>
*	date:   3-Mar-2008
*/
#ifndef VDLIB_BOX_H
#define VDLIB_BOX_H

#include <vdlib/Common.h>

namespace vdlib {

class Box
{
public:
	vr::vec3f center;  // Center point
	vr::vec3f axis[3]; // Orthonormal basis
	vr::vec3f extents; // Half-lengths along each basis' axis

	/**
	*  Input is a vr::vec3 array of size 8.
	*  Vertices are computed as follows:
	*     7+------+6
	*     /|     /|
	*    / |    / |
	*   / 3+---/--+2
	* 4+------+5 /    y  z
	*  | /    | /     | /
	*  |/     |/      |/
	* 0+------+1      *---x
	*/
	void computeVertices( vr::vec3f* vertices ) const;

	// Convenience method to get the axis that has the greatest extents
	const vr::vec3f& getLongestAxis() const;
};

} // namespace vdlib

#endif // VDLIB_BOX_H_
