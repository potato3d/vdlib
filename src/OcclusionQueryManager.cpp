#include <vdlib/OcclusionQueryManager.h>
#include <vdlib/Node.h>
#include <vdlib/OpenGL.h>

using namespace vdlib;

void OcclusionQueryManager::init( const TreeBuilder::Statistics& stats )
{
	vr::vectorExactResize( _queryIds, stats.nodeCount );
	glGenQueriesARB( stats.nodeCount, &_queryIds[0] );
}

void OcclusionQueryManager::beginBoundingVolumeQuery( Node* node )
{
	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
	glDepthMask( GL_FALSE );
	glDisable( GL_LIGHTING );
	glBeginQueryARB( GL_SAMPLES_PASSED_ARB, _queryIds[node->getId()] );

	_queryQueue.push_back( node );
}

void OcclusionQueryManager::endBoundingVolumeQuery()
{
	glEnable( GL_LIGHTING );
	glDepthMask( GL_TRUE );
	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
	glEndQueryARB( GL_SAMPLES_PASSED_ARB );
}

void OcclusionQueryManager::beginGeometryQuery( Node* node )
{
	glBeginQueryARB( GL_SAMPLES_PASSED_ARB, _queryIds[node->getId()] );
	_queryQueue.push_back( node );
}

void OcclusionQueryManager::endGeometryQuery()
{
	glEndQueryARB( GL_SAMPLES_PASSED_ARB );
}

bool OcclusionQueryManager::done() const
{
	return _queryQueue.empty();
}

Node* OcclusionQueryManager::popFrontNode()
{
	Node* frontNode = _queryQueue.front();
	_queryQueue.pop_front();
	return frontNode;
}

bool OcclusionQueryManager::frontResultAvailable() const
{
	unsigned int result;
	glGetQueryObjectuivARB( _queryIds[_queryQueue.front()->getId()], GL_QUERY_RESULT_AVAILABLE_ARB, &result );
	return ( result == GL_TRUE );
}

unsigned int OcclusionQueryManager::getQueryResult( Node* node ) const
{
	unsigned int result;
	glGetQueryObjectuivARB( _queryIds[node->getId()], GL_QUERY_RESULT_ARB, &result );
	return result;
}
