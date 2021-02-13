#include <vdlib/Node.h>

using namespace vdlib;

Node::Node()
{
	_id = 0;
	_parent = NULL;
}

Node::Node( int id )
{
	_id = id;
	_parent = NULL;
}

int Node::getId() const
{
	return _id;
}

Node* Node::getParent()
{
	return _parent;
}

void Node::setLeftChild( Node* child )
{
	child->_parent = this;
	_leftChild = child;
}

Node* Node::getLeftChild()
{
	return _leftChild.get();
}

void Node::setRightChild( Node* child )
{
	child->_parent = this;
	_rightChild = child;
}

Node* Node::getRightChild()
{
	return _rightChild.get();
}

bool Node::isLeaf() const
{
	return !_leftChild.valid() && !_rightChild.valid();
}

Box& Node::getBoundingBox()
{
	return _bbox;
}

GeometryVector& Node::getGeometries()
{
	return _geometries;
}
