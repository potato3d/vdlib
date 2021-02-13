#include <vdlib/SceneData.h>
#include <vdlib/RawNode.h>
#include <vdlib/BoxFactory.h>
#include <vdlib/Geometry.h>

using namespace vdlib;

void SceneData::beginScene()
{
	_sceneRoot = new RawNode();
}

void SceneData::beginGeometry( Geometry* geometry )
{
	// Create geometry info for new geometry
	GeometryInfo* di = new GeometryInfo();
	di->verticesStart = _sceneRoot->getVertices().size();
	di->verticesSize = 0;
	di->geometry = geometry;

	// Store it in root node
	_sceneRoot->getGeometryInfos().push_back( di );
}

void SceneData::addVertices( const float* vertices, int size )
{
	std::vector<float>& destVertices = _sceneRoot->getVertices();

	// Reserve space for new vertices
	unsigned int previousSize = destVertices.size();
	destVertices.resize( previousSize + size );

	// Append to current buffer vertices
	std::copy( vertices, vertices + size, destVertices.begin() + previousSize );

	// Increment vertex size in GeometryInfo
	_sceneRoot->getGeometryInfos().back()->verticesSize += size;
}

void SceneData::addVertices( const double* vertices, int size )
{
	std::vector<float>& destVertices = _sceneRoot->getVertices();

	// Reserve space for new vertices
	unsigned int previousSize = destVertices.size();
	destVertices.resize( previousSize + size );

	// Append to current buffer vertices
	for( int src = 0, dst = previousSize; src < size; ++src, ++dst )
		destVertices[dst] = (float)vertices[src];

	// Increment vertex size in GeometryInfo
	_sceneRoot->getGeometryInfos().back()->verticesSize += size;
}

void SceneData::transformVertices( const float* matrix )
{
	vr::mat4f mat( matrix );

	// No change
	if( mat.isIdentity() )
		return;

	// Only update vertices for current geometry
	int verticesStart = _sceneRoot->getGeometryInfos().back()->verticesStart;
	std::vector<float>& destVertices = _sceneRoot->getVertices();

	for( int i = verticesStart; i < (int)destVertices.size(); i+=3 )
	{
		// Get vertex
		vr::vec3f vertex( &destVertices[i] );

		// Apply transform
		mat.transform( vertex );

		// Save new values
		destVertices[i]   = vertex.x;
		destVertices[i+1] = vertex.y;
		destVertices[i+2] = vertex.z;
	}
}

const float* SceneData::getCurrentVertices() const
{
	// Pointer to the beginning of current geometry's vertices
	int verticesStart = _sceneRoot->getGeometryInfos().back()->verticesStart;
	return &_sceneRoot->getVertices()[verticesStart];
}

void SceneData::endGeometry()
{
	// Get geometry information
	GeometryInfo* currInfo = _sceneRoot->getGeometryInfos().back().get();

	int vertStart = currInfo->verticesStart;
	int vertSize = currInfo->verticesSize;

	// Create bounding volume using current vertices only
	BoxFactory::createBox( currInfo->geometry->getBoundingBox(), &_sceneRoot->getVertices()[vertStart], vertSize );
}

void SceneData::endScene()
{
	// Save memory
	vr::vectorTrim( _sceneRoot->getVertices() );
	vr::vectorTrim( _sceneRoot->getGeometryInfos() );
}

RawNode* SceneData::getSceneNode()
{
	return _sceneRoot.get();
}
