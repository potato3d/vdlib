/**
*	Main API to store geometric information from client.
*	author: Paulo Ivson <psantos@tecgraf.puc-rio.br>
*	date:   3-Mar-2008
*/
#ifndef _VDLIB_SCENEDATA_H_
#define _VDLIB_SCENEDATA_H_

#include <vdlib/Common.h>
#include <vdlib/RawNode.h>

namespace vdlib {

class SceneData
{
public:
	// Create a new scene node to store all geometries
	void beginScene();

	// Start sending information for a new geometry
	// Takes ownership of geometry pointer and store it
	void beginGeometry( Geometry* geometry );

	// Send geometric information for current geometry
	void addVertices( const float*  vertices, int size );
	void addVertices( const double* vertices, int size );

	// Convenience method to apply given 4x4 transformation to all vertices added thus far
	void transformVertices( const float* matrix );

	// Get vertices for current geometry thus far
	const float* getCurrentVertices() const;

	// End current geometry, create bounding box
	void endGeometry();

	// Called once at the end, when all geometries have been added
	// At this point, given sceneRoot is ready for hierarchy construction
	void endScene();

	// Used by TreeBuilder to get initial scene node with all geometries
	RawNode* getSceneNode();

private:
	vr::ref_ptr<RawNode> _sceneRoot;
};

} // namespace vdlib

#endif // _VDLIB_SCENEDATA_H_
