/**
*	General includes and definitions for VDLIB.
*	author: Paulo Ivson <psantos@tecgraf.puc-rio.br>
*	date:   3-Mar-2008
*/
#ifndef _VDLIB_COMMON_H_
#define _VDLIB_COMMON_H_

#include <vr/algebra_types.h>
#include <vr/refcounting.h>
#include <vr/stl_utils.h>

namespace vdlib
{
	// Forward declarations
	class Box;
	class BoxFactory;
	class Distance;
	class EigenSolver;
	class FrustumCuller;
	class Geometry;
	class GeometryInfo;
	class IFrustumCallback;
	class IOcclusionCallback;
	class Intersection;
	class MinMax;
	class Node;
	class OcclusionCuller;
	class OcclusionQueryManager;
	class OpenGL;
	class Plane;
	class PreOrderIterator;
	class RawNode;
	class SceneData;
	class Statistics;
	class TreeBuilder;

} // namespace vdlib

#endif // _VDLIB_COMMON_H_
