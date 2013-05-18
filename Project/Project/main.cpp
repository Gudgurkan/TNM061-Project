/* Author: Stefan Gustavson (stegu@itn.liu.se) 2013
* This code is in the public domain.
*/
#include <iostream>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lib/shader.hpp"
#include <vector>
#include <algorithm>
#include "lib/Objectloader.hpp"
#include "lib/controls.hpp"
#include "Object.h"
#include "lib/moveLight.h"

using namespace glm;

void renderB(Object &obj)
{
	obj.RenderObject();
}

void deleteB(Object &obj)
{
	obj.deleteBuffers();
}

int main () 
{
	// -----------------------------------------
	// Initialize program
	// -----------------------------------------

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

	// Create an opengl window
	if(!glfwOpenWindow (1024, 768, 8, 8, 8, 8, 32, 0, GLFW_WINDOW))
	{
		fprintf( stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return 1;
	}

	// Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	
	// -----------------------------------------
	// Set parametres for program
	// -----------------------------------------
	
	glfwSetWindowTitle( "Lightning Demo" );
	glfwEnable( GLFW_STICKY_KEYS );
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" ); 
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	// -----------------------------------------
	// Add objects
	// -----------------------------------------

	std::vector<Object> objects;

	// Add first object
	Object cylinder("cylinder.obj");	
	cylinder.translateObject(-6.0, -1.0, 0.0);
	cylinder.scaleObject(1.0, 5.0, 1.0);
	cylinder.BindBuffers();

	// Add second cylinder object
	Object cylinder2("cylinder.obj");	
	cylinder2.translateObject(-2.0, -1.0, 0.0);
	cylinder2.scaleObject(1.0, 5.0, 1.0);
	cylinder2.BindBuffers();

	// Add third cylinder object
	Object cylinder3("cylinder.obj");
	cylinder3.translateObject(2.0, -1.0, 0.0);
	cylinder3.scaleObject(1.0, 5.0, 1.0);
	cylinder3.BindBuffers();

	// Add second object
	Object sphere("sphere.obj");
	sphere.flipNormals();
	sphere.scaleObject(0.15f, 0.15f, 0.15f); 
	sphere.translateObject(0.0f, 0.0f, 0.0f);
	sphere.BindBuffers();

	// Add floor
	Object floor("floor.obj");
	floor.BindBuffers();

	objects.push_back(cylinder);
	objects.push_back(cylinder2);
	objects.push_back(cylinder3);
	//objects.push_back(cube);
	objects.push_back(floor);

	// -----------------------------------------
	// Handle lights
	// -----------------------------------------

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	GLuint LightID2 = glGetUniformLocation(programID, "LightPosition_worldspace2");
	
	// Color position
	vec3 lightPos2 = vec3(3.0,1.0,2.0);
	glUniform3f(LightID2, lightPos2.x, lightPos2.y, lightPos2.z);

	// -----------------------------------------
	// Main loop
	// -----------------------------------------
	
	do{	
		// -----------------------------------------
		// Update program
		// -----------------------------------------
	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen
		glUseProgram(programID);							// Use our shader

		// The sin and cos-terms make the lightsource feel ALIVE! 
		glm::vec3 lightPos = computeLightFromInputs() * addCircularMotion();
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		updateMatrices();

		// -----------------------------------------
		// Handle view transformation matrices
		// -----------------------------------------

		mat4 Projection = getProjectionMatrix();
		mat4 View = getViewMatrix();
		mat4 Model      = mat4(1.0f); // Model matrix : an identity matrix (model will be at the origin)
		mat4 MVP = Projection * View * Model; // Our ModelViewProjection : multiplication of our 3 matrices

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]); 
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);

		// Render objects
		for_each(objects.begin(), objects.end(), renderB);

		// -----------------------------------------
		// Handle light transformation
		// -----------------------------------------

		mat4 Model2 = translate(mat4(1.0f), lightPos);
		
		//mat4 Model      = mat4(1.0f); // Model matrix : an identity matrix (model will be at the origin)
		mat4 MVP2 = Projection * View * Model2; // Our ModelViewProjection : multiplication of our 3 matrices

		// Send our transformation to the currently bound shader, in the "MVP2" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]); 
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model2[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);

		sphere.RenderObject();

		// -----------------------------------------
		// Swap buffers, used for double buffering. Very nice.
		// -----------------------------------------

		glfwSwapBuffers();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS && glfwGetWindowParam( GLFW_OPENED ) );

	// -----------------------------------------
	// Cleanup
	// -----------------------------------------

	for_each(objects.begin(), objects.end(), deleteB);
	glDeleteProgram(programID);
	glfwTerminate();

	return 0;
}
