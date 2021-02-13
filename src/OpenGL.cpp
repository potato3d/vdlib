#include <vdlib/OpenGL.h>
#include <iostream>

using namespace vdlib;

bool OpenGL::init()
{
	// Check for general extension support
	GLenum err = glewInit();
	if( err != GLEW_OK )
	{
		// Problem: glewInit failed, something is seriously wrong.
		std::cout << "Error: " << glewGetErrorString( err ) << std::endl;
		return false;
	}
	return true;
}
