#include <GL/glfw.h>
#include <iostream>
#include <glm/glm.hpp>
#include "moveLight.h"
#include <ctime>

using namespace glm;

//Movementspeed of the lightsource
float lightSpeed = 0.05f;

//Starting position of lightsource
float x = 4;
float y = 0.5;
float z = 4;

vec3 computeLightFromInputs(){

	// Only called first time
	static double lastTime = glfwGetTime();
		
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
		
	// Move light in z-axis
	if (glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS){
		z += lightSpeed;
	}
	// Move light in z-axis
	if (glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS){
		z -= lightSpeed;
	}
	// Move light in x-axis
	if (glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS){
		x -= lightSpeed;
	}
	// Move light in x-axis
	if (glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS){
		x += lightSpeed;
	}
	
	lastTime = currentTime;

	return vec3(x,y,z);
}