#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <vector>

class Object
{
	public:
		//Renderclass(); //Den gillar in constructorerna :S
		//~Renderclass();
		Object(	std::vector<glm::vec3>,
				std::vector<glm::vec2>,
				std::vector<glm::vec3>);
		GLuint vertexbuffer;
		GLuint colorbuffer;
				
		void RenderObject();
		void brutalBuffer(GLuint vbuffer, GLuint cbuffer);
		void BindBuffers();
		void deleteBuffers();

	private:
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;

};

#endif
