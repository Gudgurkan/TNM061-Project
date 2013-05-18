#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <vector>

class Object
{
	public:		
		Object(const char *path); //konstruktor
		GLuint vertexbuffer;
		GLuint colorbuffer;
		GLuint normalbuffer;
				
		void RenderObject();
		void BindBuffers();		
		void deleteBuffers();	

		void translateObject(float x, float y, float z);
		void scaleObject(float x, float y, float z);
		glm::vec3 getCenter();
		void flipNormals();

	private:
		int sizeVertices;
		glm::vec3 center;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
};

#endif
