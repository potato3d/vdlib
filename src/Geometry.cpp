#include <vdlib/Geometry.h>

using namespace vdlib;

Geometry::Geometry()
{
	_id = -1;
}

void Geometry::setId( int id )
{
	_id = id;
}

int Geometry::getId() const
{
	return _id;
}

Box& Geometry::getBoundingBox()
{
	return _bbox;
}

const Box& Geometry::getBoundingBox() const
{
	return _bbox;
}
