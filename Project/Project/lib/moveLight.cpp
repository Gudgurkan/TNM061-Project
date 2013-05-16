#include <GL/glfw.h>
#include <iostream>
#include <glm/glm.hpp>
#include "moveLight.h"
#include <ctime>

using namespace glm;

//Movementspeed of the lightsource
float lightSpeed = 2.0f;

//Starting position of lightsource
float x = 4;
float y = 0.5;
float z = 4;
float n = 0;

vec3 computeLightFromInputs(){

	// Only called first time
	static double lastTime = glfwGetTime();
		
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
		
	// Move light in z-axis
	if (glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS){
		z -= lightSpeed*deltaTime;
	}
	// Move light in z-axis
	if (glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS){
		z += lightSpeed*deltaTime;
	}
	// Move light in x-axis
	if (glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS){
		x += lightSpeed*deltaTime;
	}
	// Move light in x-axis
	if (glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS){
		x -= lightSpeed*deltaTime;
	}
	
	lastTime = currentTime;

	return vec3(x,y,z);
}

vec3 addCircularMotion()
{	
	n += 0.01;	

	return vec3(1 + sin(n)/40, (1 + (sin(n))/4), 1 + cos(n)/40);
}