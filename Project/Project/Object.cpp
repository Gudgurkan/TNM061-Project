#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include "Object.h"
#include <glm/glm.hpp>
#include "lib/Objectloader.hpp"


Object::Object(const char *path)
{
	bool res = loadObject(path, vertices, uvs, normals);
}


void Object::BindBuffers()
{
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
}

void Object::RenderObject()
{
	// 1st attribute buffer
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(  //Contains data from the previously provided pointer
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer); //Colorbuffer has been bound to the buffer earlier
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	//DRAW
	glDrawArrays(GL_TRIANGLES, 0, normals.size()); 

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);	
}

void Object::deleteBuffers()
{
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteBuffers(1, &normalbuffer);
}

void Object::translateObject(float tx, float ty, float tz)
{	
	float centerX = 0.0;
	float centerY = 0.0;
	float centerZ = 0.0;

	for(int i = 0; i < vertices.size(); i++)
	{
		vertices[i].x = vertices[i].x + tx;
		vertices[i].y = vertices[i].y + ty;
		vertices[i].z = vertices[i].z + tz;

		centerX += vertices[i].x;
		centerY += vertices[i].y;
		centerZ += vertices[i].z;
	}

	center = glm::vec3(centerX/vertices.size(), centerY/vertices.size(), centerZ/vertices.size());	
}

void Object::scaleObject(float tx, float ty, float tz)
{	
	for(int i = 0; i < vertices.size(); i++)
	{
		vertices[i].x = vertices[i].x * tx;
		vertices[i].y = vertices[i].y * ty;
		vertices[i].z = vertices[i].z * tz;
	}
}

glm::vec3 Object::getCenter()
{
	return center;
}

void Object::flipNormals()
{
	for(int i = 0; i < normals.size(); i++)
	{
		normals[i].x = normals[i].x * -1;
		normals[i].y = normals[i].y * -1;
		normals[i].z = normals[i].z * -1;
	}
}