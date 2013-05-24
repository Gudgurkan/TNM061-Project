#include <iostream>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "controls.hpp"

using namespace glm;

mat4 ViewMat;
mat4 ProjectionMat;

float speed = 2.0f;
const vec3 right = vec3(1, 0, 0);
const vec3 forward = vec3(0, 0, 1);

// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
//float verticalAngle = -3.14f/4;		Mickesprovinkel
float verticalAngle = -2.0f/4;			// Teodor busar runt i koden
vec3 direction(
		0, 
		sin(verticalAngle),
		-cos(verticalAngle)
);

//vec3 position = vec3( 0, 6, 6);		Mickesprovinkel
vec3 position = vec3( 0, 8, 20);		// Teodor busar runt i koden


mat4 getViewMatrix(){
	return ViewMat;
}
mat4 getProjectionMatrix(){
	return ProjectionMat;
}

void updateMatrices()
{
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();

	float timePassed = (float)(lastTime - currentTime);

	// Move forward
	if (glfwGetKey( 'W' ) == GLFW_PRESS){
		position += forward * timePassed * speed;
	}
	// Move backward
	if (glfwGetKey( 'S' ) == GLFW_PRESS){
		position -= forward * timePassed * speed;
	}
	// Strafe right
	if (glfwGetKey( 'A' ) == GLFW_PRESS){
		position += right * timePassed * speed;
	}
	// Strafe left
	if (glfwGetKey( 'D' ) == GLFW_PRESS){
		position -= right * timePassed * speed;
	}

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMat = perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

	ViewMat = lookAt(
		position,
		position + direction,
		vec3(0,1,0)
	);

	lastTime = currentTime;
}
