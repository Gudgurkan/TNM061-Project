/* Author: Stefan Gustavson (stegu@itn.liu.se) 2013
* This code is in the public domain.
*/
#include <iostream>
#include <GL/glfw.h>

using namespace std;

void setupViewport () {
	int width , height ;

	glfwGetWindowSize ( &width , &height );
	glViewport (0, 0, width, height); // The entire window
}

int main () 
{
	GLboolean running = GL_TRUE ; // Main loop exits when this is set to GL_FALSE
	glfwInit();

	if(!glfwOpenWindow (256, 256, 8, 8, 8, 8, 32, 0, GLFW_WINDOW))
	{
		glfwTerminate () ; // glfwOpenWindow failed , quit the program.
		return 1;
	}

	glfwSwapInterval (0) ; // Do not wait for screen refresh between frames

	while( running )
	{
		setupViewport() ;
		glClearColor(0.3f ,0.3f ,0.3f ,0.0f) ;
		glClear(GL_COLOR_BUFFER_BIT);

		// --- Rendering code should go here ---

		glfwSwapBuffers() ;

		if( glfwGetKey( GLFW_KEY_ESC ) || ! glfwGetWindowParam (GLFW_OPENED)) 
		{
			running = GL_FALSE ;
		}
	}

	glfwTerminate();
	return 0;
}