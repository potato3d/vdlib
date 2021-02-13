#include <vdlib/OcclusionCuller.h>
#include <vdlib/Node.h>
#include <vdlib/OpenGL.h>
#include <vdlib/Intersection.h>
#include <vdlib/Distance.h>

using namespace vdlib;

//////////////////////////////////////////////////////////////////////////
// Global functions
void renderBoundingBox( Node* node )
{
	vr::vec3f vertices[8];
	node->getBoundingBox().computeVertices( vertices );

	glBegin( GL_QUADS );
	// -z
	glVertex3fv( vertices[0].ptr );
	glVertex3fv( vertices[1].ptr );
	glVertex3fv( vertices[5].ptr );
	glVertex3fv( vertices[4].ptr );

	// +x
	glVertex3fv( vertices[1].ptr );
	glVertex3fv( vertices[2].ptr );
	glVertex3fv( vertices[6].ptr );
	glVertex3fv( vertices[5].ptr );

	// +z
	glVertex3fv( vertices[2].ptr );
	glVertex3fv( vertices[3].ptr );
	glVertex3fv( vertices[7].ptr );
	glVertex3fv( vertices[6].ptr );

	// -x
	glVertex3fv( vertices[3].ptr );
	glVertex3fv( vertices[0].ptr );
	glVertex3fv( vertices[4].ptr );
	glVertex3fv( vertices[7].ptr );

	// +y
	glVertex3fv( vertices[4].ptr );
	glVertex3fv( vertices[5].ptr );
	glVertex3fv( vertices[6].ptr );
	glVertex3fv( vertices[7].ptr );

	// -y
	glVertex3fv( vertices[0].ptr );
	glVertex3fv( vertices[3].ptr );
	glVertex3fv( vertices[2].ptr );
	glVertex3fv( vertices[1].ptr );

	glEnd();
}

//////////////////////////////////////////////////////////////////////////
// OcclusionCuller
OcclusionCuller::OcclusionCuller()
: _distanceQueue( ClosestToViewpoint( _occlusionInfo ) )
{
	_visibilityThreshold = 0;
	_frameId = 0;
}

void OcclusionCuller::init( const TreeBuilder::Statistics& stats )
{
	_queryManager.init( stats );
	vr::vectorExactResize( _occlusionInfo, stats.nodeCount );
}

void OcclusionCuller::updateViewerParameters( const float* viewMatrix, const float* projectionMatrix )
{
	vr::mat4f view( viewMatrix );

	// Set viewpoint position as inverse of LookAt translation
	_viewpoint.set( -view( 3, 0 ), -view( 3, 1 ), -view( 3, 2 ) );

	// Multiply the viewpoint position by the transpose of the rotation part of the view matrix.
	// It is equivalent of doing view * _viewpoint (without transposing, it would be _viewpoint * view).
	// In other words, we need to apply the inverse transform. Since rotation is orthonormal, inverse == transpose.
	view.transform3x3( _viewpoint );

	vr::mat4f proj( projectionMatrix );
	vr::mat4f viewProj;
	viewProj.product( view, proj );
	const float* mat = viewProj.ptr();

	// Set near plane in World Space (view*proj)
	_nearPlane.set( mat[3] + mat[2], mat[7] + mat[6], mat[11] + mat[10], mat[15] + mat[14] );
}

void OcclusionCuller::setVisibilityThreshold( unsigned int numPixels )
{
	_visibilityThreshold = numPixels;
}

unsigned int OcclusionCuller::getVisibilityThreshold() const
{
	return _visibilityThreshold;
}

void OcclusionCuller::traverse( Node* node, IOcclusionCallback* callback )
{
	Node* currentNode;
	bool queryAvailabe;

	_distanceQueue.push( node );
	++_frameId;

	// Traverse hierarchy and render visible nodes
	while( !_distanceQueue.empty() || !_queryManager.done() )
	{
		//-- PART 1: Process finished occlusion queries for current frame
		while( !_queryManager.done() && 
			( ( queryAvailabe = _queryManager.frontResultAvailable() ) || _distanceQueue.empty() ) )
		{
			// Current node
			currentNode = _queryManager.popFrontNode();

			// Get occlusion query result from OpenGL
			unsigned int visiblePixels = _queryManager.getQueryResult( currentNode );

			// If visible
			if( visiblePixels > _visibilityThreshold )
			{
				// Update this node's and its parent's visibility classifications
				pullUpVisibility( currentNode );

				// Get occlusion information for this node
				OcclusionInfo& currentInfo = _occlusionInfo[currentNode->getId()];

				// Only need to render nodes that haven't already been rendered in current frame
				if( currentInfo.lastRendered < _frameId )
				{
					currentInfo.lastRendered = _frameId;
					callback->draw( currentNode );
					pushChildren( currentNode );
				}
			}
		}

		//-- PART 2: Hierarchical traversal
		if( _distanceQueue.empty() )
			continue;

		// Get next node to be traversed
		currentNode = _distanceQueue.top();
		_distanceQueue.pop();

		// Skip invalid nodes
		if( !callback->isValid( currentNode ) )
			continue;

		// Get occlusion information for this node
		OcclusionInfo& currentInfo = _occlusionInfo[currentNode->getId()];

		// If a bounding volume intersects the near plane, we may have wrong query results.
		// This is because of back-face culling, we will (wrongly) see through the internal sides of the box.
		// In this case, we may find the box to be invisible since a part of it is not being rendered at all.
		// Therefore, skip occlusion query and traverse node.
		if( Intersection::between( _nearPlane, currentNode->getBoundingBox() ) == 0 )
		{
			pullUpVisibility( currentNode );
			currentInfo.lastVisited = _frameId;
			currentInfo.lastRendered = _frameId;
			callback->draw( currentNode );
			pushChildren( currentNode );
		}
		else
		{
			// Identify previously visible nodes (temporal coherence)
			bool wasVisible = currentInfo.visible && ( currentInfo.lastVisited == ( _frameId - 1 ) );

			// Reset node's visibility classification
			currentInfo.visible = false;

			// Update node's visited flag
			currentInfo.lastVisited = _frameId;

			// A previously visible interior node is classified as an opened node.
			// A previously invisible interior node or any kind of leaf node is classified as a termination node.
			if( wasVisible )
			{
				// Will render anyway for conservative culling
				currentInfo.lastRendered = _frameId;

				// Identify internal node
				if( !currentNode->isLeaf() )
				{
					// Opened node (visible internal node)
					// Skip testing for occlusion query
					pushChildren( currentNode );
				}
				else
				{
					// Termination node (visible leaf node)
					// Note: will query bounding volume if it is being rendered
					_queryManager.beginGeometryQuery( currentNode );
					callback->draw( currentNode );
					_queryManager.endGeometryQuery();
				}
			}
			else
			{
				// Termination node (invisible node)
				// A previously invisible node (leaf or interior) needs to have its bounding volume tested for occlusion
				_queryManager.beginBoundingVolumeQuery( currentNode );
				renderBoundingBox( currentNode );
				_queryManager.endBoundingVolumeQuery();
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Private
//////////////////////////////////////////////////////////////////////////

// OcclusionInfo
OcclusionCuller::OcclusionInfo::OcclusionInfo()
{
	lastVisited = -1;
	lastRendered = -1;
	visible = false;
	distanceToViewpoint = 0.0f;
}

// ClosestToViewpoint
OcclusionCuller::ClosestToViewpoint::ClosestToViewpoint( const OcclusionCuller::OcclusionInfoVector& info )
: _info( info )
{
	// empty
}

bool OcclusionCuller::ClosestToViewpoint::operator()( const Node* first, const Node* second ) const
{
	return _info[first->getId()].distanceToViewpoint > _info[second->getId()].distanceToViewpoint;
}

// OcclusionCuller
void OcclusionCuller::pushChildren( Node* node )
{
	Node* child = node->getLeftChild();
	if( child != NULL )
	{
		_occlusionInfo[child->getId()].distanceToViewpoint = Distance::between( _viewpoint, child->getBoundingBox() );
		_distanceQueue.push( child );
	}

	child = node->getRightChild();
	if( child != NULL )
	{
		_occlusionInfo[child->getId()].distanceToViewpoint = Distance::between( _viewpoint, child->getBoundingBox() );
		_distanceQueue.push( child );
	}
}

// Update ancestors visibility
void OcclusionCuller::pullUpVisibility( Node* node )
{
	while( node != NULL )
	{
		OcclusionInfo& info = _occlusionInfo[node->getId()];
		if( info.visible == true )
			break;

		info.visible = true;
		node = node->getParent();
	}
}
