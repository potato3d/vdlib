/**
*	Manage sending and retrieving occlusion queries to/from OpenGL.
*	author: Paulo Ivson <psantos@tecgraf.puc-rio.br>
*	date:   10-Mar-2008
*/
#ifndef _VDLIB_OCCLUSIONQUERYMANAGER_H_
#define _VDLIB_OCCLUSIONQUERYMANAGER_H_

#include <vdlib/Common.h>
#include <vdlib/TreeBuilder.h>
#include <deque>

namespace vdlib {

// Warning: assumes node ids are consecutive and start with zero (TreeBuilder guarantees this).
class OcclusionQueryManager
{
public:
	// Reallocate query ids for each node
	void init( const TreeBuilder::Statistics& stats );

	// Disable color and depth writes
	void beginBoundingVolumeQuery( Node* node );
	void endBoundingVolumeQuery();

	void beginGeometryQuery( Node* node );
	void endGeometryQuery();

	// Any queries left processing?
	bool done() const;

	// Front node represents oldest issued query
	Node* popFrontNode();
	bool frontResultAvailable() const;
	unsigned int getQueryResult( Node* node ) const;

private:
	std::vector<unsigned int> _queryIds;
	std::deque<Node*> _queryQueue;
};

} // namespace vdlib

#endif // _VDLIB_OCCLUSIONQUERYMANAGER_H_
