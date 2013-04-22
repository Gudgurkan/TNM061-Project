#include <stdio.h>
#include <stdlib.h>

#include <GL/glfw.h>

void setupViewport(){
	int width, height;
	GLuint vertexArrayID, vertexBufferID, indexBufferID;

	glfwGetWindowSize(&width, &height);
	glViewport(0,0,width,height);

	static const GLfloat vertex_array_data[] = {
		-1.0f, -1.0f, 0.0f,	//First vertex xyz
		1.0f, -1.0f, 0.0f,	//Second vertex
		0.5f, 1.0f, 0.0f	//Third vertex
	};

	static const GLuint index_array_data[] = {
		0,1,2
	};


}

int main()
{
	GLboolean running = GL_TRUE;

	glfwInit();
	if(!glfwOpenWindow(256, 256, 8, 8, 8, 8, 32, 0, GLFW_WINDOW)){
		glfwTerminate();
		return 0;
	}
	printf("GL renderer:     %s\n", glGetString(GL_RENDERER));
	glfwSwapInterval(1);

	while(running)
	{
		setupViewport();
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Rendering code

		glfwSwapBuffers();
		if(glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED)){
			running = GL_FALSE;
		}
	}

	glfwTerminate();
	return 0;
}