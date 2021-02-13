#include <vdlib/RawNode.h>
#include <vdlib/Node.h>
#include <vdlib/Geometry.h>
#include <vdlib/BoxFactory.h>

using namespace vdlib;

RawNode::RawNode()
{
	_treeDepth = 0;
	_node = new Node();
}

RawNode::RawNode( int id )
{
	_treeDepth = 0;
	_node = new Node( id );
}

void RawNode::setLeftChild( RawNode* child )
{
	child->_treeDepth = _treeDepth + 1;
	_leftChild = child;
	_node->setLeftChild( child->getHierarchyNode() );
}

RawNode* RawNode::getLeftChild()
{
	return _leftChild.get();
}

void RawNode::setRightChild( RawNode* child )
{
	child->_treeDepth = _treeDepth + 1;
	_rightChild = child;
	_node->setRightChild( child->getHierarchyNode() );
}

RawNode* RawNode::getRightChild()
{
	return _rightChild.get();
}

void RawNode::removeLeftChild()
{
	_leftChild = NULL;
}

void RawNode::removeRightChild()
{
	_rightChild = NULL;
}

int RawNode::getTreeDepth() const
{
	return _treeDepth;
}

std::vector<float>& RawNode::getVertices()
{
	return _vertices;
}

const std::vector<float>& RawNode::getVertices() const
{
	return _vertices;
}

GeometryInfoVector& RawNode::getGeometryInfos()
{
	return _geometryInfos;
}

const GeometryInfoVector& RawNode::getGeometryInfos() const
{
	return _geometryInfos;
}

Node* RawNode::getHierarchyNode()
{
	return _node.get();
}

void RawNode::computeBoundingBox()
{
	// If only 1 geometry, reuse its box
	if( _geometryInfos.size() == 1 )
		_node->getBoundingBox() = _geometryInfos[0]->geometry->getBoundingBox();
	else 
		BoxFactory::createBox( _node->getBoundingBox(), &_vertices[0], _vertices.size() );
}

void RawNode::assignGeometriesToHierarchyNode()
{
	GeometryVector& geometries = _node->getGeometries();

	vr::vectorExactResize( geometries, _geometryInfos.size() );

	for( unsigned int i = 0; i < _geometryInfos.size(); ++i )
		geometries[i] = _geometryInfos[i]->geometry;
}
