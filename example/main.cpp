/**
*	Example viewer for VDLIB.
*	author: Paulo Ivson <psantos@tecgraf.puc-rio.br>
*	date:   11-Mar-2008
*/

#include <vdlib/OpenGL.h>
#include <vdlib/BoxFactory.h>
#include <vdlib/SceneData.h>
#include <vdlib/TreeBuilder.h>
#include <vdlib/FrustumCuller.h>
#include <vdlib/OcclusionCuller.h>

#include <vr/random.h>
#include <vr/timer.h>
#include <vr/string.h>
#include <vector>
#include "Teapot.h"

#include <gl/glut.h>
#include <gl/wglew.h>

#include <iostream>

/************************************************************************/
/* Classes and types                                                    */
/************************************************************************/

static void drawTeapot( int id );
static void drawBox( const vdlib::Box& box, float r, float g, float b );

enum DrawMode
{
	Draw_Simple,
	Draw_FrustumCulling,
	Draw_OcclusionCulling,
	Draw_All
};

enum DebugMask
{
	Debug_Off           = 0,
	Debug_NodeBoxes     = 1,
	Debug_GeometryBoxes = 2,
	Debug_All           = 3
};

class RenderCallback : public vdlib::IFrustumCallback, public vdlib::IOcclusionCallback
{
public:
	RenderCallback() : _frustumCuller( NULL ), _debugMask( Debug_Off ) {}

	// Frustum culling only
	virtual void inside( vdlib::Node* node )
	{
		draw( node );
	}

	// Occlusion culling
	virtual void draw( vdlib::Node* node )
	{
		if( _debugMask & Debug_NodeBoxes )
			drawBox( node->getBoundingBox(), 0.0f, 1.0f, 0.0f );

		const vdlib::GeometryVector& geometries = node->getGeometries();

		for( unsigned int i = 0; i < geometries.size(); ++i )
		{
			drawTeapot( geometries[i]->getId() );

			if( _debugMask & Debug_GeometryBoxes )
				drawBox( geometries[i]->getBoundingBox(), 0.0f, 0.0f, 1.0f );
		}
	}

	// Occlusion culling + frustum culling
	virtual bool isValid( vdlib::Node* node )
	{
		if( _frustumCuller != NULL )
			return _frustumCuller->contains( node );
		else
			return true;
	}

	// If valid pointer, use frustum culling with occlusion culling
	// Else, disable VFC with occlusion
	void setFrustumCuller( vdlib::FrustumCuller* frustum )
	{
		_frustumCuller = frustum;
	}

	// Toggle bounding box debugging
	void setDebugMode( DebugMask mask )
	{
		_debugMask = mask;
	}

	DebugMask getDebugMode() const
	{
		return _debugMask;
	}

private:
	vdlib::FrustumCuller* _frustumCuller;
	DebugMask _debugMask;
};

class Teapot
{
public:
	vr::vec3f color;
	std::vector<float> vertices;
};

/************************************************************************/
/* Global variables                                                     */
/************************************************************************/

// Specify acceleration algorithm to use
static DrawMode s_drawMode = Draw_Simple;
static vr::String s_drawModeString = "Alg: none";

// Geometry total provided by user
static int s_geometryCount = 10;
static float s_geometryScale = 5.0f;
static std::vector<Teapot> s_teapots;

// Main hierarchy root
static vr::ref_ptr<vdlib::Node> s_sceneRoot;

// Acceleration algorithms
static vdlib::FrustumCuller   s_frustumCuller;
static vdlib::OcclusionCuller s_occlusionCuller;

// My rendering callback
static RenderCallback s_renderCallback;

// Camera manipulation
static double s_walkSpeed = 25.0;
static double s_lookSpeed = 0.005;
static vr::vec2f s_prevMousePos = vr::vec2f( 0.0, 0.0 );

static bool s_moveForward  = false;
static bool s_moveBackward = false;
static bool s_moveLeft     = false;
static bool s_moveRight    = false;
static bool s_moveUp       = false;
static bool s_moveDown     = false;

static vr::mat4f s_viewMatrix;
static vr::mat4f s_projMatrix;

// Frames per second
static int s_frameCounter = 0;
static double s_lastFrameTime = 0.0;
static double s_accumFrameTime = 0.0;
static vr::Timer s_fpsTimer;
static vr::String s_fpsMsg;
static double s_minFrameTime = 0.0033; // given by maximum fps by user
static vr::Timer s_renderTimer;
static double s_lastRenderTime = 0.0;

// Miscellaneous
static bool s_wireframeActive = false;

/************************************************************************/
/* Utility functions                                                    */
/************************************************************************/
static void initTeapots()
{
	s_teapots.resize( s_geometryCount );

	for( unsigned int i = 0; i < s_teapots.size(); ++i )
	{
		// Allocate vertices
		s_teapots[i].vertices.resize( vdlib::NUM_TEAPOT_VERTICES * 3 );

		// Set random color
		s_teapots[i].color.set( vr::Random::real( 0.2, 1.0 ), 
			                    vr::Random::real( 0.2, 1.0 ), 
								vr::Random::real( 0.2, 1.0 ) );
	}
}

static void drawTeapot( int id )
{
	const std::vector<float>& vertices = s_teapots[id].vertices;

	// Send color
	glColor3fv( s_teapots[id].color.ptr );

	// Send geometry
	unsigned int i = 0;
	while( i < vdlib::NUM_TEAPOT_INDICES )
	{
		glBegin( GL_TRIANGLE_STRIP );
		while( vdlib::TEAPOT_INDICES[i] != vdlib::STRIP_END )
		{
			int index = vdlib::TEAPOT_INDICES[i] * 3;
			glNormal3fv( vdlib::TEAPOT_NORMALS + index );
			glVertex3fv( &vertices[0] + index );
			i++;
		}
		glEnd();
		i++; // skip strip end flag
	}
}

static void drawBox( const vdlib::Box& box, float r, float g, float b  )
{
	if( !s_wireframeActive )
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glDisable( GL_LIGHTING );

	// When geometry and node boxes overlap, show geometry boxes
	glDepthFunc( GL_LEQUAL );

	glColor3f( r, g, b );

	vr::vec3f vertices[8];
	box.computeVertices( vertices );

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

	glDepthFunc( GL_LESS );
	glEnable( GL_LIGHTING );
	if( !s_wireframeActive )
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

static void resetCamera()
{
	s_viewMatrix.makeLookAt( vr::vec3f( 0.0, 0.0, 20.0 ), vr::vec3f( 0.0, 0.0, 0.0 ), vr::vec3f( 0.0, 1.0, 0.0 ) );
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( s_viewMatrix.ptr() );
}

static void moveCamera( const vr::mat4f& transform )
{
	s_viewMatrix.product( s_viewMatrix, transform );
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( s_viewMatrix.ptr() );
}

static void updateCameraMovement()
{
	vr::vec3f translation( 0, 0, 0 );

	// Set translation
	if( s_moveForward )
		translation.z += 1.0f;

	if( s_moveLeft )
		translation.x += 1.0f;

	if( s_moveBackward )
		translation.z -= 1.0f;

	if( s_moveRight )
		translation.x -= 1.0f;

	if( s_moveUp )
		translation.y -= 1.0f;

	if( s_moveDown )
		translation.y += 1.0f;

	translation.normalize();
	translation *= s_walkSpeed;

	// Accumulate translation
	vr::mat4f translationMatrix;
	translationMatrix.makeTranslation( translation * s_lastFrameTime );

	// Accumulate to current view matrix
	moveCamera( translationMatrix );
}

static void displayTextLine( const char* s, float x, float y )
{
	glRasterPos3d( x, y, 0.8 );

	for( unsigned int i = 0; s[i]; ++i )
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, s[i] );
}

static void displayScreenMessage()
{
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );

	glColor3f( 1.0f, 1.0f, 1.0f );

	// Show FPS
	displayTextLine( s_fpsMsg.toCharArray(), -0.95f, 0.90f );

	// Show current draw mode
	displayTextLine( s_drawModeString.toCharArray(), -0.95f, 0.80f );

	// Show current debug mode
	vr::String debugString;
	switch( s_renderCallback.getDebugMode() )
	{
	case Debug_Off:
		debugString = "Box: off";
		break;
	case Debug_NodeBoxes:
		debugString = "Box: node";
		break;
	case Debug_GeometryBoxes:
		debugString = "Box: geometry";
	    break;
	case Debug_All:
		debugString = "Box: node+geometry";
		break;
	default:
		debugString = "Box: off";
		break;
	}

	displayTextLine( debugString.toCharArray(), -0.95f, 0.70f );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
}

/************************************************************************/
/* Main functions                                                       */
/************************************************************************/
static void getSceneParametersFromUser()
{
	std::cout << "Enter number of geometries: ";
	std::cin >> s_geometryCount;

	std::cout << "Enter individual geometry scale: ";
	std::cin >> s_geometryScale;

	double maxFps = 100000.0;
	
	std::cout << "Enter maximum frames per second: ";
	std::cin >> maxFps;

	s_minFrameTime = 1.0 / maxFps;
}

static void createScene()
{
	// Allocate memory for teapot vertices
	initTeapots();

	vdlib::SceneData sceneData;
	vr::mat4f transform;
	vr::mat4f aux;

	// Set bounding box type to use
	//vdlib::BoxFactory::setDefaultBoxType( vdlib::BoxFactory::Type_Obb );

	// Begin sending geometry to vdlib
	sceneData.beginScene();

	for( int i = 0; i < s_geometryCount; ++i )
	{
		// Compute random transformation

		// Constant scale
		transform.makeScale( s_geometryScale, s_geometryScale, s_geometryScale );

		// Random rotation in local coordinates
		float angle = vr::Random::real( 0.0, 2.0 * vr::Mathd::PI );
		aux.makeRotation( angle, vr::Random::realInIn(),
			                     vr::Random::realInIn(),
								 vr::Random::realInIn() );

		transform.product( transform, aux );

		// Random translation
		aux.makeTranslation( vr::Random::real( -10.0,   10.0 ),
							 vr::Random::real( -10.0,   10.0 ),
							 vr::Random::real(   0.0, -100.0 ) );

		transform.product( transform, aux );

		// Create geometry
		vdlib::Geometry* geom = new vdlib::Geometry();
		geom->setId( i );

		// Send it to scene data
		sceneData.beginGeometry( geom );
		sceneData.addVertices( vdlib::TEAPOT_VERTICES, vdlib::NUM_TEAPOT_VERTICES * 3 );
		sceneData.transformVertices( transform.ptr() );

		// Store transformed vertices back for rendering
		const float* vertices = sceneData.getCurrentVertices();
		std::copy( vertices, vertices + vdlib::NUM_TEAPOT_VERTICES * 3, &s_teapots[i].vertices[0] );

		// End current geometry
		sceneData.endGeometry();
	}

	// End current scene
	sceneData.endScene();

	// Build hierarchy
	vdlib::TreeBuilder builder;
	s_sceneRoot = builder.createTree( sceneData );

	// Get stats and setup frustum & occlusion culling
	s_frustumCuller.init( builder.getStatistics() );
	s_occlusionCuller.init( builder.getStatistics() );
}

// Draw scene
static void drawScene()
{
	if( !s_sceneRoot.valid() )
		return;

	// Update viewer parameters
	vr::mat4f aux;
	aux.product( s_viewMatrix, s_projMatrix );
	s_frustumCuller.updateFrustumPlanes( aux.ptr() );
	s_occlusionCuller.updateViewerParameters( s_viewMatrix.ptr(), s_projMatrix.ptr() );

	// Traverse hierarchy and draw
	switch( s_drawMode )
	{
	case Draw_Simple:
		for( vdlib::PreOrderIterator itr( s_sceneRoot.get() ); !itr.done(); itr.next() )
			s_renderCallback.draw( itr.current() );
		break;

	case Draw_FrustumCulling:
		s_frustumCuller.traverse( s_sceneRoot.get(), &s_renderCallback );
		break;

	case Draw_OcclusionCulling:
	case Draw_All:
		s_occlusionCuller.traverse( s_sceneRoot.get(), &s_renderCallback );
	    break;

	default:
	    break;
	}
}

static void beginFrame()
{
	// Update general timers and counters
	s_lastFrameTime = s_fpsTimer.restart();
	s_accumFrameTime += s_lastFrameTime;

	++s_frameCounter;

	// Compute fps with new timer values
	if( s_accumFrameTime >= 0.5 )
	{
		double fps = s_frameCounter / s_accumFrameTime;
		s_fpsMsg.format( "%.2f fps    %.2f ms", fps, s_lastFrameTime * 1000.0 );

		s_frameCounter = 0;
		s_accumFrameTime = 0.0;
	}

	// Begin timing current frame
	s_renderTimer.restart();
}

static void endFrame()
{
	// End timing render
	s_lastRenderTime = s_renderTimer.elapsed();

	// Limit maximum frames per second to reduce CPU usage
	// We force the current frame to take longer than normal
	if( s_lastRenderTime < s_minFrameTime )
		Sleep( ( s_minFrameTime - s_lastRenderTime ) * 1000.0 );
}

// Display callback
static void display()
{
	// Begin draw operations
	beginFrame();

	// Clear buffers
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Set view matrix
	updateCameraMovement();

	// Draw scene
	drawScene();

	// Update on-screen user information
	displayScreenMessage();

	// End of draw operations
	endFrame();

	// Display current frame
	glutSwapBuffers();
}

// Idle callback for performance measurements
static void idle()
{
	display();
}

// Reshape callback
static void reshape( int w, int h )
{
	static bool first = true;
	if( first )
	{
		glClearColor( 0, 0, 0, 1 );
		glEnable( GL_DEPTH_TEST );

		glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
		glEnable( GL_COLOR_MATERIAL );

		glEnable( GL_NORMALIZE );

		// Setup headlight
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		glEnable( GL_LIGHTING );
		glEnable( GL_LIGHT0 );
		float lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightfv( GL_LIGHT0, GL_POSITION, lightPos );

		// Reset camera
		resetCamera();

		// Make sure context is created prior to initializing OpenGL functionalities
		bool ok = vdlib::OpenGL::init();
		if( !ok )
		{
			printf( "Error initializing OpenGL, aborting...\n" );
			exit( 1 );
		}

		// Disable V-Sync for benchmarking
		// Attention! Only works under Windows
		wglSwapIntervalEXT( 0 );

		// Create scene with valid OpenGL context
		createScene();

		// Reset global timer
		s_fpsTimer.restart();

		first = false;
	}

	glMatrixMode( GL_PROJECTION );
	s_projMatrix.makePerspective( 65.0, (double)w/(double)h, 0.1, 1000.0 );
	glLoadMatrixf( s_projMatrix.ptr() );

	glViewport( 0, 0, w, h );
}

// Key down callback
static void keyDown( unsigned char key, int x, int y )
{
	switch( key )
	{
	// Camera movement
	case 'w':
		{
			s_moveForward = true;
			break;
		}
	case 'a':
		{
			s_moveLeft = true;
			break;
		}
	case 's':
		{
			s_moveBackward = true;
			break;
		}
	case 'd':
		{
			s_moveRight = true;
			break;
		}
	case 'r':
		{
			s_moveUp = true;
			break;
		}
	case 'f':
		{
			s_moveDown = true;
			break;
		}


	// Drawing modes
	case '1':
		s_drawMode = Draw_Simple;
		s_renderCallback.setFrustumCuller( NULL );
		s_drawModeString = "Alg: none";
		break;

	case '2':
		s_drawMode = Draw_FrustumCulling;
		s_renderCallback.setFrustumCuller( NULL );
		s_drawModeString = "Alg: VFC";
		break;

	case '3':
		s_drawMode = Draw_OcclusionCulling;
		s_renderCallback.setFrustumCuller( NULL );
		s_drawModeString = "Alg: CHC";
		break;

	case '4':
		s_drawMode = Draw_All;
		s_renderCallback.setFrustumCuller( &s_frustumCuller );
		s_drawModeString = "Alg: VFC+CHC";
		break;

	// Debug modes
	case 'b':
		{
			DebugMask mask = (DebugMask)( (int)s_renderCallback.getDebugMode() + 1 );
			if( mask > Debug_All )
				mask = Debug_Off;
			s_renderCallback.setDebugMode( mask );
			break;
		}

	// Space key: reset viewer
	case 32:
		resetCamera();
		break;

	// Esc key: exit application
	case 27:
		exit( 0 );
		break;

	// Wireframe
	case 'z':
		s_wireframeActive ^= true;
		if( s_wireframeActive )
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		else
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		break;

	default:
	    return;
	}

	glutPostRedisplay();
}

// Key up callback
static void keyUp( unsigned char key, int x, int y )
{
	switch( key )
	{
	// Camera movement
	case 'w':
		{
			s_moveForward = false;
			break;
		}
	case 'a':
		{
			s_moveLeft = false;
			break;
		}
	case 's':
		{
			s_moveBackward = false;
			break;
		}
	case 'd':
		{
			s_moveRight = false;
			break;
		}
	case 'r':
		{
			s_moveUp = false;
			break;
		}
	case 'f':
		{
			s_moveDown = false;
			break;
		}

	default:
		return;
	}

	glutPostRedisplay();
}

// Mouse callback
void mouse( int button, int state, int x, int y )
{
	if( state == GLUT_DOWN )
		s_prevMousePos.set( x, y );
}

// Mouse drag callback
void drag( int x, int y )
{
	vr::vec2f currentMousePos( x, y );
	vr::vec2f delta = currentMousePos - s_prevMousePos;
	s_prevMousePos = currentMousePos;

	float rotateX = delta.y * s_lookSpeed;
	float rotateY = delta.x * s_lookSpeed;

	// Set rotation
	vr::mat4f rotationMatrix;
	rotationMatrix.makeRotation( rotateX, vr::vec3f( 1, 0, 0 ),
		                         rotateY, vr::vec3f( 0, 1, 0 ),
		                               0, vr::vec3f( 0, 0, 1 ) );

	moveCamera( rotationMatrix );
}

//////////////////////////////////////////////////////////////////////////
// Main entry point
//////////////////////////////////////////////////////////////////////////
int main( int argc, char* argv[] )
{
	// Open GLUT
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
	glutInitWindowSize( 640, 480 );

#ifdef _DEBUG
	int windowId = glutCreateWindow( "VdViewer - Debug Mode" );
#else
	int windowId = glutCreateWindow( "VdViewer" );
#endif

	// Disable key repeat to get a single state change
	glutIgnoreKeyRepeat( 1 );

	glutReshapeFunc( reshape );
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyDown );
	glutKeyboardUpFunc( keyUp );
	glutMouseFunc( mouse );
	glutMotionFunc( drag );

	// Before creating rendering window, for clarity
	getSceneParametersFromUser();

	// Interact
	glutMainLoop();
	return 0;
}
