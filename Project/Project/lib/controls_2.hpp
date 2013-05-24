#ifndef CONTROLS_HPP
#define CONTROLS_HPP

using namespace glm;

vec3 computeLightFromInputs();
vec3 addCircularMotion();
vec3 getPosView();
mat4 getViewMatrix();
mat4 getProjectionMatrix();

#endif