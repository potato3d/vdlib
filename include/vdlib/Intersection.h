/**
*	General intersection algorithms.
*	author: Paulo Ivson <psantos@tecgraf.puc-rio.br>
*	date:   7-Mar-2008
*/
#ifndef _VDLIB_INTERSECTION_H_
#define _VDLIB_INTERSECTION_H_

#include <vdlib/Common.h>

namespace vdlib {

class Intersection
{
public:
	/**
	 *	Return code:
	 *		Totally inside:  +1
	 *		Totally outside: -1
	 *		Intersected:      0
	 *
	 *	Plane must be in Hessian Normal Form.
	 */
	static int between( const Plane& plane, const Box& box );
};

} // namespace vdlib

#endif // _VDLIB_INTERSECTION_H_
