/**
*	Represents a view-frustum: implements culling and traversal algorithms.
*	author: Paulo Ivson <psantos@tecgraf.puc-rio.br>
*	date:   7-Mar-2008
*/
#ifndef _VDLIB_FRUSTUMCULLER_H_
#define _VDLIB_FRUSTUMCULLER_H_

#include <vdlib/Common.h>
#include <vdlib/TreeBuilder.h>
#include <vdlib/Plane.h>
#include <vdlib/PreOrderIterator.h>

namespace vdlib {

// Traversal callback
class IFrustumCallback
{
public:
	// Called for every node that is found inside the view frustum
	virtual void inside( Node* node ) = 0;
};

// Main frustum culling algorithms
// Warning: assumes node ids are consecutive and start with zero (TreeBuilder guarantees this).
class FrustumCuller
{
public:
	// Reallocate per-node culling information.
	void init( const TreeBuilder::Statistics& stats );

	// Extracts all 6 frustum planes from matrix.
	// If matrix equals Projection, planes will be defined in Eye Space.
	// If matrix equals View * Projection, planes will be defined in World Space.
	void updateFrustumPlanes( const float* matrix );

	// Tests frustum planes in the following order: near, left, right, bottom, top, far.
	// Implements spatial coherence (don't test planes that parent node was found to be totally inside).
	// Implements temporal coherence (tests each node against its respective previous culling plane).
	bool contains( Node* node );

	// Traverse hierarchy performing view-frustum culling
	void traverse( Node* node, IFrustumCallback* callback );

private:
	class CullingInfo
	{
	public:
		CullingInfo();

		unsigned int planeId;
		unsigned int planeMask;
	};

	unsigned int getParentCullingMask( Node* node ) const;

	Plane _planes[6];
	std::vector<CullingInfo> _cullingInfo;
	PreOrderIterator _itr;
};

} // namespace vdlib

#endif // _VDLIB_FRUSTUMCULLER_H_
