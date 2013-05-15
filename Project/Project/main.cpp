/* Author: Stefan Gustavson (stegu@itn.liu.se) 2013
* This code is in the public domain.
*/
#include <iostream>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lib/shader.hpp"
#include "Cube.h"
#include <vector>
#include "lib/Objectloader.hpp"
#include "Renderclass.h"

using namespace glm;

int main () 
{
	//Initialize GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	//Set OpenGl Version
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create an opengl window
	if(!glfwOpenWindow (1024, 768, 8, 8, 8, 8, 32, 0, GLFW_WINDOW))
	{
		fprintf( stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return 1;
	}

	//initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	//Set parametres for program
	glfwSetWindowTitle( "Lightning Demo" );
	glfwEnable( GLFW_STICKY_KEYS );
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 

	//Prepare the graphics card for a vertex array
	//GLuint VertexArrayID;
	//glGenVertexArrays(1, &VertexArrayID);
	//glBindVertexArray(VertexArrayID);

	GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" ); // Create and compile our GLSL program from the shaders
	GLuint MatrixID = glGetUniformLocation(programID, "MVP"); // Get a handle for our "MVP" uniform
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f); // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	glm::mat4 Model      = glm::mat4(1.0f); // Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 MVP        = Projection * View * Model; // Our ModelViewProjection : multiplication of our 3 matrices

	//Read .obj file
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals;
	bool res = loadObject("cube.obj", vertices, uvs, normals);

	/*GLuint vertexbuffer;
	GLuint colorbuffer;

	Renderclass bufferTo;
	bufferTo.brutalBuffer(vertexbuffer, colorbuffer);*/

	
	//Buffer for vertices
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	////Buffer for vertices
	//GLuint vertexbuffer;
	//glGenBuffers(1, &vertexbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	////Buffer for colors at every vertex
	//GLuint colorbuffer;
	//glGenBuffers(1, &colorbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);


	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen
		glUseProgram(programID); // Use our shader
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]); // Send our transformation to the currently bound shader, in the "MVP" uniform
		
		Renderclass renderObjectInstance;			// DET BALLAR UR!	
		renderObjectInstance.RenderObject(vertexbuffer, colorbuffer);

		glfwSwapBuffers(); // Swap buffers, used for double buffering. Very nice.

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS && glfwGetWindowParam( GLFW_OPENED ) );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	//glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}