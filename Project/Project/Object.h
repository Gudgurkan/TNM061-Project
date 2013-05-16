#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <vector>

class Object
{
	public:
		//Renderclass(); //Den gillar in constructorerna :S
		//~Renderclass();
		Object(const char *path); //konstruktor
		GLuint vertexbuffer;
		GLuint colorbuffer;
		GLuint normalbuffer;
				
		void RenderObject();
		void BindBuffers();
		void deleteBuffers();

	private:
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
};

#endif
