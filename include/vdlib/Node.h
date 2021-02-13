/**
*	Main hierarchy node.
*	author: Paulo Ivson <psantos@tecgraf.puc-rio.br>
*	date:   4-Mar-2008
*/
#ifndef _VDLIB_NODE_H_
#define _VDLIB_NODE_H_

#include <vdlib/Common.h>
#include <vdlib/Geometry.h>
#include <vdlib/Box.h>

namespace vdlib {

// Hierarchy node used in main hierarchy
class Node : public vr::RefCounted
{
public:
	Node();

	// Only TreeBuilder should use this
	explicit Node( int id );

	// Internal identifier 
	int getId() const;

	// Hierarchy
	Node* getParent();

	void setLeftChild( Node* child );
	Node* getLeftChild();

	void setRightChild( Node* child );
	Node* getRightChild();
	
	bool isLeaf() const;

	// Geometry
	Box& getBoundingBox();
	GeometryVector& getGeometries();

private:
	int _id;

	Node* _parent;
	vr::ref_ptr<Node> _leftChild;
	vr::ref_ptr<Node> _rightChild;

	Box _bbox;
	GeometryVector _geometries;
};

} // namespace vdlib

#endif // _VDLIB_NODE_H_
