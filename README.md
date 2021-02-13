# vdlib
Visibility Determination Library: hierarchical frustum and occlusion culling algorithms

Frustum culling implementation based on the following papers:
* [Fast Extraction of Viewing Frustum Planes from the WorldView-Projection Matrix](https://www.gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf)
* [Optimized View Frustum Culling Algorithms for Bounding Boxes](http://www.cse.chalmers.se/~uffe/vfc_bbox.pdf)

Occlusion culling implementation based on the following papers:
* [Coherent Hierarchical Culling: Hardware Occlusion Queries Made Useful](https://www.vrvis.at/publications/pdfs/PB-VRVis-2004-034.pdf)
* [Near Optimal Hierarchical Culling: Performance Driven Use of Hardware Occlusion Queries](http://diglib.eg.org/handle/10.2312/EGWR.EGSR06.207-214)
* [Occlusion Culling with Statistically Optimized Occlusion Queries](https://www.researchgate.net/publication/221546605_Occlusion_Culling_with_Statistically_Optimized_Occlusion_Queries)

The full description of this library is available in this [report](https://github.com/potato3d/vdlib/blob/main/doc/ModelosMassivosRev.pdf), [paper](https://github.com/potato3d/vdlib/blob/main/doc/poster_final.pdf) and [poster](https://github.com/potato3d/vdlib/blob/main/doc/Relatorio_Final_de_Projeto.pdf) (all in brazilian portuguese).

# External dependencies
The library uses OpenGL for rendering, including functionalities like occlusion queries for acceleration algorithms.

List of dependencies:
* GLEW: Opengl Extension Wrangler
** Import OpenGL extensions
* VrBase: Basic library from Virtual Reality Group at Tecgraf
** Basic types for Linear Algebra
** Timer, Random, etc
    
All external dependencies are inside the folders:
    /depend
        /include
        /lib
    
# Main library: VDLIB
Main source code can be found at:
    /include
        /vdlib
    /src

Entire implementation is under the namespace 'vdlib', organized as follows:

* Frustum Culling
** FrustumCuller

* Occlusion Culling
** OcclusionCuller
** OcclusionQueryManager

* Utilities
** Distance
** EigenSolver
** Intersection
** Statistics

* Scene
** Geometry
** SceneData

* Bounding Box
** Box
** BoxFactory

* Hierarchy
** Node
** RawNode
** TreeBuilder
** PreOrderIterator

* OpenGL
** Plane

# Example: VdViewer
In addition to main library's dependencies, the example viewer uses GLUT for window management. 

The source code is at:
    /example

Main commands:
    Camera
        Move:  W, A, S, D, R, F
        Turn:  Mouse1 + Drag, Q, E
        Reset: Space
        
    Rendering
        Wireframe: Z
        Toggle bounding-box debug: B
            off           - no bounding box
            node          - draw node bounding boxes (green)
            geometry      - draw geometry bounding boxes (blue)
            node+geometry - draw all bounding boxes
            
    Algorithms
        No acceleration:   1
        Frustum culling:   2
        Occlusion culling: 3
        Frustum+Occlusion: 4
        
    Miscellaneous
        Exit: Esc

# Results

Here are some images and times for scene walkthrough:

![scenes](https://github.com/potato3d/vdlib/blob/main/imgs/scenes.png "Scenes and performance")

# Build
Projects for Microsoft Visual Studio 2005 (VC8) are ready to use at:
    /visualstudio