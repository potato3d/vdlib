#include <vdlib/FrustumCuller.h>
#include <vdlib/Node.h>
#include <vdlib/Intersection.h>

using namespace vdlib;

FrustumCuller::CullingInfo::CullingInfo()
{
	planeId = 0;
	planeMask = 0xFFFFFFFF;
}

//////////////////////////////////////////////////////////////////////////
// Frustum Culler
//////////////////////////////////////////////////////////////////////////

void FrustumCuller::init( const TreeBuilder::Statistics& stats )
{
	vr::vectorExactResize( _cullingInfo, stats.nodeCount );
}

void FrustumCuller::updateFrustumPlanes( const float* matrix )
{
	/**
	*	IMPORTANT: accessing matrix as transpose since OpenGL matrix is column-major
	*/
	// Near clipping plane
	_planes[0].set( matrix[3] + matrix[2], matrix[7] + matrix[6], matrix[11] + matrix[10], matrix[15] + matrix[14] );

	// Left clipping plane
	_planes[1].set( matrix[3] + matrix[0], matrix[7] + matrix[4], matrix[11] + matrix[8], matrix[15] + matrix[12] );

	// Right clipping plane
	_planes[2].set( matrix[3] - matrix[0], matrix[7] - matrix[4], matrix[11] - matrix[8], matrix[15] - matrix[12] );

	// Bottom clipping plane
	_planes[3].set( matrix[3] + matrix[1], matrix[7] + matrix[5], matrix[11] + matrix[9], matrix[15] + matrix[13] );

	// Top clipping plane
	_planes[4].set( matrix[3] - matrix[1], matrix[7] - matrix[5], matrix[11] - matrix[9], matrix[15] - matrix[13] );

	// Far clipping plane
	_planes[5].set( matrix[3] - matrix[2], matrix[7] - matrix[6], matrix[11] - matrix[10], matrix[15] - matrix[14] );

	// Normalize planes
	_planes[0].normalize();
	_planes[1].normalize();
	_planes[2].normalize();
	_planes[3].normalize();
	_planes[4].normalize();
	_planes[5].normalize();
}

bool FrustumCuller::contains( Node* node )
{
	CullingInfo& nodeInfo = _cullingInfo[node->getId()];

	// This mask indicates for which frustum planes the parent Node has been found to be totally inside.
	// Therefore, there is no need to test the current Node's bounding volume against these same planes.
	unsigned int planeMask = getParentCullingMask( node );

	// First of all, check if parent Node is not already totally inside view frustum.
	// If it is, just seeking which frustum plane we still need to test ( which is none ) would be a waste of time.
	if( planeMask == 0xFFFFFFC0 )
	{
		// In this case, just update the current Node's plane mask, reset the culling plane and leave
		nodeInfo.planeMask = planeMask;
		return true;
	}

	// Node's bounding volume for plane intersection
	const Box& box = node->getBoundingBox();

	// If result < 0, the bounding volume is on the negative half-space of the plane (totally outside the view frustum)
	// If result > 0, the bounding volume is on the positive half-space of the plane (totally inside the view frustum)
	// If result == 0, the bounding volume is intercepted by the plane
	int result;

	// Index of frustum plane previously responsible for culling this Node
	unsigned int cullingPlane = nodeInfo.planeId;

	// Used in selecting a plane that still needs to be tested
	unsigned int selectorMask = 1 << cullingPlane;

	// Now, test bounding box intersection for previous culling plane first.
	// But only if parent Node was not already found to be totally inside this particular plane.
	if( selectorMask & planeMask )
	{
		result = Intersection::between( _planes[cullingPlane], box );

		if( result < 0 )
		{
			nodeInfo.planeMask = planeMask;
			return false;
		}
		else if( result > 0 )
		{
			// No need to test this plane anymore
			planeMask ^= selectorMask;
		}
	}

	// Continue testing bounding box for each remaining view frustum planes
	for( unsigned int i = 0, selectorMask = 1; i < 6; ++i, selectorMask <<= 1 )
	{
		// Check if we need to test the current plane
		if( selectorMask & planeMask )
		{
			result = Intersection::between( _planes[i], box );

			if( result < 0 )
			{
				// Store new culling mask and update culling plane for future speedup
				nodeInfo.planeMask = planeMask;
				nodeInfo.planeId = i;
				return false;
			}
			else if( result > 0 )
			{
				// No need to test this plane anymore
				planeMask ^= selectorMask;
			}
		}
	}

	// Finally, store new culling mask for my children
	nodeInfo.planeMask = planeMask;

	// If we got this far, than bounding volume is at least partially contained by the view frustum
	return true;
}

void FrustumCuller::traverse( Node* node, IFrustumCallback* callback )
{
	_itr.begin( node );
	while( !_itr.done() )
	{
		if( contains( _itr.current() ) )
		{
			callback->inside( _itr.current() );
			_itr.next();
		}
		else
		{
			_itr.skip();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Private
unsigned int FrustumCuller::getParentCullingMask( Node* node ) const
{
	if( node->getParent() != NULL )
		return _cullingInfo[node->getParent()->getId()].planeMask;
	else
		return 0xFFFFFFFF;
}
