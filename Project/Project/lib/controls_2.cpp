#include <iostream>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "controls.hpp"
#include <ctime>

using namespace glm;

mat4 ViewMat;
mat4 ProjectionMat;

float speed = 2.0f;
float mouseSpeed = 0.003f;

float horizontalAngle = 0.0f;
float verticalAngle = -3.14f/4;

float n = 0;

vec3 posView = vec3( 0, 4.5, -8);	
vec3 posBall = vec3( 0, 0.5, -4);


mat4 getViewMatrix(){
	return ViewMat;
}
mat4 getProjectionMatrix(){
	return ProjectionMat;
}

vec3 getPosView(){
	return posView;
}

vec3 computeLightFromInputs()
{
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();

	float timePassed = (float)(lastTime - currentTime);

	// Get mouse position
	int xpos, ypos;
	glfwGetMousePos(&xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetMousePos(1024/2, 768/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024/2 - xpos );
		
	// Direction vector
	vec3 direction(
		sin(horizontalAngle), 
		sin(-3.14f/4),
		cos(horizontalAngle)
	);

	// Right vector
	vec3 right = vec3(
		sin(horizontalAngle + 3.14f/2.0f), 
		0,
		cos(horizontalAngle + 3.14f/2.0f)
	);

	//forward vector
	vec3 forward = vec3(
		cos(horizontalAngle + 3.14f/2.0f), 
		0,
		-sin(horizontalAngle + 3.14f/2.0f)
	);

	// Move forward
	if (glfwGetKey( 'W' ) == GLFW_PRESS){
		posBall += forward * timePassed * speed;
	}
	// Move backward
	if (glfwGetKey( 'S' ) == GLFW_PRESS){
		posBall -= forward * timePassed * speed;
	}
	// Strafe right
	if (glfwGetKey( 'D' ) == GLFW_PRESS){
		posBall += right * timePassed * speed;
	}
	// Strafe left
	if (glfwGetKey( 'A' ) == GLFW_PRESS){
		posBall -= right * timePassed * speed;
	}
		posView.x = posBall.x -4*direction.x;
		posView.z = posBall.z -4*direction.z;
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMat = perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

	ViewMat = lookAt(
		posView,
		posView + direction,
		vec3(0,1,0)
	);

	lastTime = currentTime;

	return vec3 (posBall);
}

vec3 addCircularMotion()
{	
	n += 0.01;	

	return vec3(1 + sin(n)/40, (1 + (sin(n))/4), 1 + cos(n)/40);
}