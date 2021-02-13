/**
*	Initial node used only during hierarchy construction.
*	author: Paulo Ivson <psantos@tecgraf.puc-rio.br>
*	date:   4-Mar-2008
*/
#ifndef _VDLIB_RAWNODE_H_
#define _VDLIB_RAWNODE_H_

#include <vdlib/Common.h>
#include <vdlib/Plane.h>

namespace vdlib {

// This stores information only needed during hierarchy construction
// Afterwards, only a reference to the actual Geometry is stored in each Node
class GeometryInfo : public vr::RefCounted
{
public:
	int verticesStart;
	int verticesSize;
	vr::ref_ptr<Geometry> geometry;
};

typedef std::vector< vr::ref_ptr<GeometryInfo> > GeometryInfoVector;

// Stores additional information than final hierarchy node
class RawNode : public vr::RefCounted
{
public:
	RawNode();
	explicit RawNode( int id );

	void setLeftChild( RawNode* child );
	RawNode* getLeftChild();

	void setRightChild( RawNode* child );
	RawNode* getRightChild();

	// Remove sub-trees that aren't needed anymore
	void removeLeftChild();
	void removeRightChild();

	int getTreeDepth() const;

	std::vector<float>& getVertices();
	const std::vector<float>& getVertices() const;

	GeometryInfoVector& getGeometryInfos();
	const GeometryInfoVector& getGeometryInfos() const;

	// This is the final node that is used for main algorithms
	Node* getHierarchyNode();

	// Recompute bounding box based on current vertices and geometry information
	void computeBoundingBox();

	// This method is called when RawNode is found to be a leaf node.
	// Copy references to all Geometries from RawNode to hierarchy Node.
	void assignGeometriesToHierarchyNode();

private:
	int _treeDepth;					   // Tree depth at this node
	vr::ref_ptr<RawNode> _leftChild;   // Temporary left child
	vr::ref_ptr<RawNode> _rightChild;  // Temporary right child

	std::vector<float> _vertices;      // Accumulated vertices for all geometries stored
	GeometryInfoVector _geometryInfos; // Geometry information

	vr::ref_ptr<Node> _node;           // Actual hierarchy node
};

} // namespace vdlib

#endif // _VDLIB_RAWNODE_H_
