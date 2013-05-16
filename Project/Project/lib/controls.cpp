#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "controls.hpp"

using namespace glm;

mat4 ViewMat;
mat4 ProjectionMat;

mat4 getViewMatrix(){
	return ViewMat;
}
mat4 getProjectionMatrix(){
	return ProjectionMat;
}

void updateMatrices()
{
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMat = perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

	ViewMat = lookAt(
		vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
		vec3(0,0,0), // and looks at the origin
		vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
}
