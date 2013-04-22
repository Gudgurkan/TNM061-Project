#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>


int width, height;
GLuint vertexArrayID, vertexBufferID, indexBufferID;

void setupViewport(){

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
	
	glGenVertexArrays(1, &vertexArrayID); //generate 1 VAO, put resulting identifier in vertexArrayID
	glGenBuffers(1, &vertexBufferID); //generate 1 buffer, put resulting identifier in vertexBufferID
	glGenBuffers(1, &indexBufferID); //generate 1 buffer, put resulting identifier in indexBufferID

	glBindVertexArray(vertexArrayID);//activate the vertex array object
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID); //Activate the vertex buffer object
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_array_data), vertex_array_data, GL_STATIC_DRAW); //Present our vertex coordiantes to OpenGL
	glEnableVertexAttribArray(0); //Enable vertex attribute array 0 to send it to the shader.
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID); //Activate the index buffer object
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_array_data), index_array_data, GL_STATIC_DRAW); //Present out vertex indices to OpenGL
	
	glBindVertexArray(0); //Deactivate the vertex array object again to be nice
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
		//Activate the vertex array object we want to draw
		glBindVertexArray(vertexArrayID);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);

		glfwSwapBuffers();
		if(glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED)){
			running = GL_FALSE;
		}
	}

	glfwTerminate();
	return 0;
}