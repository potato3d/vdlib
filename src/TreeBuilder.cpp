#include <vdlib/TreeBuilder.h>
#include <vdlib/SceneData.h>
#include <vdlib/Node.h>
#include <vdlib/RawNode.h>
#include <vdlib/Geometry.h>
#include <vdlib/Distance.h>

using namespace vdlib;

void TreeBuilder::Statistics::reset()
{
	leafCount = 0;
	nodeCount = 0;
	treeDepth = 0;
}

// TreeBuilder implementation
TreeBuilder::TreeBuilder()
{
	_stats.reset();
	_maxTreeDepth = 24;
	_minVertexCount = 3000;
	_minGeometryCount = 1;
}

vr::ref_ptr<Node> TreeBuilder::createTree( SceneData& sceneData )
{
	RawNode* sceneNode = sceneData.getSceneNode();

	// Reset statistics
	_stats.reset();

	// TODO: to be verified
	// Set maximum tree depth based on number of geometries on scene
	_maxTreeDepth = (int)( 1.2 * vr::log2( (double)sceneNode->getGeometryInfos().size() ) + 2.0 );

	// Count root node
	++_stats.nodeCount;

	// Recursive hierarchy construction
	recursiveCreateHierarchy( sceneNode );

	// Return stored scene root
	return vr::ref_ptr<Node>( sceneNode->getHierarchyNode() );
}

const TreeBuilder::Statistics& TreeBuilder::getStatistics() const
{
	return _stats;
}

//////////////////////////////////////////////////////////////////////////
// Private
//////////////////////////////////////////////////////////////////////////
void TreeBuilder::recursiveCreateHierarchy( RawNode* node )
{
	// Create node's bounding box
	node->computeBoundingBox();

	// Heuristic criteria to stop recursive construction algorithm
	if( checkTerminateRecursion( node ) != Condition_Ok )
	{
		// Return new leaf node
		setLeafNode( node );
		return;
	}

	// Find "best" split position and orientation
	Plane splitPlane;
	findSplitPlane( splitPlane, node );

	// Partition geometries around chosen plane
	Condition result = partitionGeometries( node, splitPlane );

	// If we subdivided successfully
	if( result == Condition_Ok )
	{
		// Recursively create hierarchy for both sub-trees
		// Delete all construction nodes except for root.
		// Actual tree will be preserved in hierarchy node inside root node.
		recursiveCreateHierarchy( node->getLeftChild() );
		node->removeLeftChild();
	
		recursiveCreateHierarchy( node->getRightChild() );
		node->removeRightChild();
	}
	else
	{
		// Failed subdivision, current node must be a leaf node.
		setLeafNode( node );
	}
}

TreeBuilder::Condition TreeBuilder::checkTerminateRecursion( RawNode* node )
{
	if( (int)node->getVertices().size() <= _minVertexCount )
		return Condition_Min_Vertex_Count;

	if( (int)node->getGeometryInfos().size() <= _minGeometryCount )
		return Condition_Min_Geometry_Count;

	if( (int)node->getTreeDepth() >= _maxTreeDepth )
		return Condition_Max_Tree_Depth;

	return Condition_Ok;
}

void TreeBuilder::findSplitPlane( Plane& plane, RawNode* node )
{
	// Average center split
	GeometryInfoVector& geometryInfos = node->getGeometryInfos();
	vr::vec3f averageCenter( 0.0f, 0.0f, 0.0f );

	// Compute center
	for( unsigned int i = 0; i < geometryInfos.size(); ++i )
		averageCenter += geometryInfos[i]->geometry->getBoundingBox().center;

	averageCenter *= 1.0f / (float)geometryInfos.size();

	// Plane normal follows the longest axis of node's bounding box.
	const vr::vec3f& normal = node->getHierarchyNode()->getBoundingBox().getLongestAxis();

	// Plane is positioned in average point of all geometries' centers.
	plane.set( normal, averageCenter );
}

TreeBuilder::Condition TreeBuilder::partitionGeometries( RawNode* node, const Plane& splitPlane )
{
	GeometryInfoVector& srcGeoms = node->getGeometryInfos();
	GeometryInfoVector leftGeoms;
	GeometryInfoVector rightGeoms;

	int leftVertexCount = 0;
	int rightVertexCount = 0;

	for( unsigned int i = 0; i < srcGeoms.size(); ++i )
	{
		GeometryInfo* geom = srcGeoms[i].get();
		
		float distanceToPlane = Distance::between( geom->geometry->getBoundingBox().center, splitPlane );

		// This way, geometries that lie on the split plane are assigned to right child only
		if( distanceToPlane < 0 )
		{
			leftVertexCount += geom->verticesSize;
			leftGeoms.push_back( geom );
		}
		else
		{
			rightVertexCount += geom->verticesSize;
			rightGeoms.push_back( geom );
		}
	}

	// If any of the children's complexity falls below minimum vertex count, we don't subdivide the current node.
	if( ( leftVertexCount < _minVertexCount ) || ( rightVertexCount < _minVertexCount ) )
		return Condition_Min_Vertex_Count;

	// Go ahead and create children
	RawNode* left  = new RawNode( _stats.nodeCount++ );
	RawNode* right = new RawNode( _stats.nodeCount++ );

	// Source vertices to be partitioned
	const std::vector<float>& vertices = node->getVertices();

	// Assign vertices to left child
	std::vector<float>& leftVertices = left->getVertices();
	leftVertices.reserve( leftVertexCount );

	for( unsigned int i = 0; i < leftGeoms.size(); ++i )
	{
		// Get iterator to source vertices
		std::vector<float>::const_iterator startItr = vertices.begin() + leftGeoms[i]->verticesStart;
		
		// Update geometry info with new vertex start index
		leftGeoms[i]->verticesStart = leftVertices.size();

		// Copy source vertices to destination
		leftVertices.insert( leftVertices.end(), startItr, startItr + leftGeoms[i]->verticesSize );
	}

	left->getGeometryInfos() = leftGeoms;

	// Assign vertices to right child
	std::vector<float>& rightVertices = right->getVertices();
	rightVertices.reserve( rightVertexCount );

	for( unsigned int i = 0; i < rightGeoms.size(); ++i )
	{
		// Get iterator to source vertices
		std::vector<float>::const_iterator startItr = vertices.begin() + rightGeoms[i]->verticesStart;

		// Update geometry info with new vertex start index
		rightGeoms[i]->verticesStart = rightVertices.size();

		// Copy source vertices to destination
		rightVertices.insert( rightVertices.end(), startItr, startItr + rightGeoms[i]->verticesSize );
	}

	right->getGeometryInfos() = rightGeoms;

	// Add children
	node->setLeftChild( left );
	node->setRightChild( right );

	// Save memory
	vr::vectorTrim( left->getGeometryInfos() );
	vr::vectorTrim( right->getGeometryInfos() );
	vr::vectorFreeMemory( node->getGeometryInfos() );

	return Condition_Ok;
}

void TreeBuilder::setLeafNode( RawNode* node )
{
	node->assignGeometriesToHierarchyNode();

	if( node->getTreeDepth() > _stats.treeDepth )
		_stats.treeDepth = node->getTreeDepth();

	++_stats.leafCount;
}
