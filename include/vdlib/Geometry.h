/**
*	Base class for a geometric entity.
*	author: Paulo Ivson <psantos@tecgraf.puc-rio.br>
*	date:   3-Mar-2008
*/
#ifndef _VDLIB_GEOMETRY_H_
#define _VDLIB_GEOMETRY_H_

#include <vdlib/Common.h>
#include <vdlib/Box.h>

namespace vdlib {

class Geometry : public vr::RefCounted
{
public:
	Geometry();

	// Client-side identifier
	void setId( int id );
	int getId() const;

	// Bounding volume
	Box& getBoundingBox();
	const Box& getBoundingBox() const;

private:
	int _id;
	Box _bbox;
};

typedef std::vector< vr::ref_ptr<Geometry> > GeometryVector;

} // namespace vdlib

#endif // _VDLIB_GEOMETRY_H_
