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
	GLuint DepthBiasID = glGetUniformLocation(programID, "DepthBiasMVP");
	GLuint ShadowMapID = glGetUniformLocation(programID, "shadowMap");

	GLuint depthProgramID = LoadShaders( "DepthRTT.vertexshader", "DepthRTT.fragmentshader" );
	GLuint depthMatrixID = glGetUniformLocation(depthProgramID, "depthMVP");

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
	// Setup shadow map
	// -----------------------------------------

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
 
	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	glDrawBuffer(GL_NONE);													// No color buffer is drawn to.
 
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) // Always check that our framebuffer is ok
		return false;

	// -----------------------------------------
	// Setup rendering shadow map to screen
	// -----------------------------------------

	GLuint quad_VertexArrayID;
	glGenVertexArrays(1, &quad_VertexArrayID);
	glBindVertexArray(quad_VertexArrayID);

	static const GLfloat g_quad_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
	};

	GLuint quad_vertexbuffer;
	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	// Create and compile our GLSL program from the shaders
	GLuint quad_programID = LoadShaders( "Passthrough.vertexshader", "SimpleTexture.fragmentshader" );
	GLuint texID = glGetUniformLocation(quad_programID, "texture");


	// -----------------------------------------
	// Main loop
	// -----------------------------------------
	
	do{	
		vec3 lightPos = computeLightFromInputs() * addCircularMotion();
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// -----------------------------------------
		// Render shadow map framebuffer
		// -----------------------------------------

		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		glViewport(0, 0, 1024, 1024); // Render on the whole framebuffer, complete from the lower left corner to the upper right

		// We don't use bias in the shader, but instead we draw back faces, 
		// which are already separated from the front faces by a small distance 
		// (if your geometry is made this way)
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(depthProgramID);

		// Compute the MVP matrix from the light's point of view
		mat4 depthProjectionMatrix = ortho<float>(10,-10,10,-10,10,-20);
		//mat4 depthViewMatrix = lookAt(lightPos, vec3(10,10,10), vec3(0,1,0)); //(LookAtPos, CenterPos, upVector)
		mat4 depthViewMatrix = lookAt(lightPos, getPosView(), vec3(0,1,0)); //(LookAtPos, CenterPos, upVector)
		// or, for spot light :
		//vec3 lightInvDir = getPosView();
		//mat4 depthProjectionMatrix = perspective<float>(45.0f, 1.0f, 2.0f, 50.0f);
		//mat4 depthViewMatrix = lookAt(lightPos + vec3(1.0,1.0,1.0), lightPos, vec3(0,1,0));
		//mat4 depthViewMatrix = lookAt(lightPos, lightPos-lightInvDir, vec3(0,1,0));

		mat4 depthModelMatrix = mat4(1.0);
		mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
 
		glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);		// Send our transformation to the currently bound shader, in the "MVP" uniform

		for_each(objects.begin(), objects.end(), renderB);

		// -----------------------------------------
		// Render to screen
		// -----------------------------------------

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0,0,1024,768); // Render on the whole framebuffer, complete from the lower left corner to the upper right

		// So that the floor is drawn as well
		glDisable(GL_CULL_FACE);
		glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);

		mat4 Projection = getProjectionMatrix();
		mat4 View = getViewMatrix();
		mat4 Model      = mat4(1.0f); // Model matrix : an identity matrix (model will be at the origin)
		mat4 MVP = Projection * View * Model; // Our ModelViewProjection : multiplication of our 3 matrices

		glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0, 
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
		);

		glm::mat4 depthBiasMVP = biasMatrix*depthMVP;

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]); 
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(DepthBiasID, 1, GL_FALSE, &depthBiasMVP[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glUniform1i(ShadowMapID, 0);

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

		// Render light point
		sphere.RenderObject();

		// -----------------------------------------
		// Optionally render the shadowmap (for debug only)
		// -----------------------------------------

		// Render only on a corner of the window (or we we won't see the real rendering...)
		glViewport(0,0,256,256);
		glUseProgram(quad_programID);
		
		glActiveTexture(GL_TEXTURE0);				// Bind our texture in Texture Unit 0
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glUniform1i(texID, 0);						// Set our "renderedTexture" sampler to user Texture Unit 0

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		// You have to disable GL_COMPARE_R_TO_TEXTURE above in order to see anything !
		glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
		glDisableVertexAttribArray(0);

		glUseProgram(programID);
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
