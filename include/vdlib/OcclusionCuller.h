/**
*	Implements Coherent Hierarchical Culling algorithm.
*	See "Coherent Hierarchical Culling: Hardware Occlusion Queries Made Useful"
*		Jiri Bittner, Michael Wimmer, Harald Piringer, Werner Purgathofer
*	author: Paulo Ivson <psantos@tecgraf.puc-rio.br>
*	date:   10-Mar-2008
*/
#ifndef _VDLIB_OCCLUSIONCULLER_H_
#define _VDLIB_OCCLUSIONCULLER_H_

#include <vdlib/Common.h>
#include <vdlib/Plane.h>
#include <vdlib/OcclusionQueryManager.h>
#include <queue>

namespace vdlib {

// Callback used to determine valid nodes to visit and rendering notification to client
class IOcclusionCallback
{
public:
	// Called whenever a node _must_ be immediately rendered.
	// Attention: this is critical for correct occlusion culling behavior!
	virtual void draw( Node* node ) = 0;

	// Called for every visited node during traversal.
	// Determine whether the node should be traversed or not (i.e. frustum culling).
	virtual bool isValid( Node* node ) { return true; }
};

// Warning: assumes node ids are consecutive and start with zero (TreeBuilder guarantees this).
// Node bounding volumes must be defined in World Space
class OcclusionCuller
{
public:
	OcclusionCuller();

	// Reallocate occlusion information for all nodes
	void init( const TreeBuilder::Statistics& stats );

	// Viewing information needs to be updated whenever camera changes
	void updateViewerParameters( const float* viewMatrix, const float* projectionMatrix );

	// Minimum resulting pixels required for a geometry to be considered "visible"
	void setVisibilityThreshold( unsigned int numPixels );
	unsigned int getVisibilityThreshold() const;

	// Traverse hierarchy performing occlusion culling
	void traverse( Node* node, IOcclusionCallback* callback );

private:
	// Store per-node occlusion information
	class OcclusionInfo
	{
	public:
		OcclusionInfo();

		int   lastVisited;			// Last time node was visited during traversal
		int   lastRendered;			// Last time node was rendered
		bool  visible;				// Last computed visibility information
		float distanceToViewpoint;	// Last computed distance to viewpoint
	};
	typedef std::vector<OcclusionInfo> OcclusionInfoVector;

	// Predicate for ordering traversal of Nodes from closest to viewpoint to farthest.
	// For use in a priority queue, where the top element has greater priority than all others.
	class ClosestToViewpoint
	{
	public:
		ClosestToViewpoint( const OcclusionInfoVector& info );

		// Predicate operator for sorting
		bool operator()( const Node* first, const Node* second ) const;

	private:
		const OcclusionInfoVector& _info;		
	};

	// Push children to distance queue
	void pushChildren( Node* node );

	// Update ancestors visibility
	void pullUpVisibility( Node* node );

	// Viewing information
	vr::vec3f _viewpoint;
	Plane _nearPlane;

	// Occlusion information
	unsigned int _visibilityThreshold;
	OcclusionInfoVector _occlusionInfo;
	OcclusionQueryManager _queryManager;

	// Priority queue for front-to-back traversal
	typedef std::priority_queue<Node*, std::vector<Node*>, ClosestToViewpoint> DistanceQueue;
	DistanceQueue _distanceQueue;
	int _frameId;
};

} // namespace vdlib

#endif // _VDLIB_OCCLUSIONCULLER_H_
