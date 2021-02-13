/**
*	Basic hierarchy builder, implements "Average Center" spatial subdivision heuristic.
*	See "OBB-Tree: A Hierarchical Structure for Rapid Interference Detection"
*		S. Gottschalk, M.C. Lin, D. Manocha
*	author: Paulo Ivson <psantos@tecgraf.puc-rio.br>
*	date:   4-Mar-2008
*/
#ifndef _VDLIB_TREEBUILDER_H_
#define _VDLIB_TREEBUILDER_H_

#include <vdlib/Common.h>

namespace vdlib {

class TreeBuilder
{
public:
	// Keep statistics during construction
	class Statistics
	{
	public:
		void reset();

		int leafCount;
		int nodeCount;
		int treeDepth;
	};

	TreeBuilder();

	// Termination heuristics
	// If the number of vertices inside a node falls below this value, the node is declared a leaf
	void setMinVertexCount( int count );

	//////////////////////////////////////////////////////////////////////////
	// Main hierarchy construction
	// Return final hierarchy node that represents the root for the entire scene
	// After the hierarchy is built, SceneData is no longer needed
	//////////////////////////////////////////////////////////////////////////
	vr::ref_ptr<Node> createTree( SceneData& sceneData );

	// Get last hierarchy stats
	const Statistics& getStatistics() const;

private:
	enum Condition
	{
		Condition_Ok,
		Condition_Min_Vertex_Count,
		Condition_Min_Geometry_Count,
		Condition_Max_Tree_Depth,
		Condition_Indivisible
	};

	// Main recursive construction
	void recursiveCreateHierarchy( RawNode* node );

	// Check end of recursion based on heuristics
	Condition checkTerminateRecursion( RawNode* node );

	// Find best split position
	void findSplitPlane( Plane& plane, RawNode* node );

	// Given a split position, try to partition the node
	// If successful, return new children. Else do nothing.
	Condition partitionGeometries( RawNode* node, const Plane& splitPlane );

	// Set a final leaf node
	void setLeafNode( RawNode* node );

	Statistics _stats;
	int _maxTreeDepth;
	int _minVertexCount;
	int _minGeometryCount;
};

} // namespace vdlib

#endif // _VDLIB_TREEBUILDER_H_
