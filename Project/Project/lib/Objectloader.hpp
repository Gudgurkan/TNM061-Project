#ifndef OBJECTLOADER_H
#define OBJECTLOADER_H

using namespace std;

bool loadObject(
	const char * path, 
	vector<glm::vec3> & out_vertices, 
	vector<glm::vec2> & out_uvs, 
	vector<glm::vec3> & out_normals
);

#endif