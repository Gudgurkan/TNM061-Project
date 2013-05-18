#include <vector>
//#include <stdio.h>
#include <fstream>
#include <string>
#include <cstring>
#include <iostream>
#include <glm/glm.hpp>
#include "Objectloader.hpp"
#include <algorithm>    // std::replace_if
#include <sstream>

bool isSlash (char c)
{
	if(c == '/')
		return true;
	else
		return false;
}

bool loadObject(
	const char * path, 
	vector<glm::vec3> & out_vertices, 
	vector<glm::vec2> & out_uvs, 
	vector<glm::vec3> & out_normals
){
	std::vector<int> vertexIndices, uvIndices, normalIndices;
	vector<glm::vec3> temp_vertices;
	vector<glm::vec2> temp_uvs;
	vector<glm::vec3> temp_normals;


	ifstream myfile (path);

    if (myfile.is_open())
    {
		int count = 0;
        while ( myfile.good() )
        {
			string index;
			myfile >> index;

			if(index.compare("v")==0)
			{
				glm::vec3 vertex;

				myfile >> vertex.x;
				myfile >> vertex.y;
				myfile >> vertex.z;

				temp_vertices.push_back(vertex);
			}
			else if(index.compare("vt")==0)
			{
				glm::vec2 textCoor;

				myfile >> textCoor.x;
				myfile >> textCoor.y;

				temp_uvs.push_back(textCoor);
			}
			else if(index.compare("vn")==0)
			{
				glm::vec3 normal;

				myfile >> normal.x;
				myfile >> normal.y;
				myfile >> normal.z;

				temp_normals.push_back(normal);
			}
			else if(index.compare("f")==0)
			{
				string line;
				getline(myfile, line);
				std::replace_if (line.begin(), line.end(), isSlash, ' ');
				int vertexIndex[3], uvIndex[3], normalIndex[3];
				std::stringstream ss;
				ss << line;

				for(int i = 0; i < 3; i++)
				{
					ss	>> vertexIndex[i]
						>> uvIndex[i];

					if(ss >> normalIndex[i])
					{
						vertexIndices.push_back(vertexIndex[i]);
						uvIndices    .push_back(uvIndex[i]);
						normalIndices.push_back(normalIndex[i]);
					}
					else
					{
						normalIndex[i] = uvIndex[i];
						vertexIndices.push_back(vertexIndex[i]);
						uvIndex[i] = 1;
						uvIndices    .push_back(uvIndex[i]);
						normalIndices.push_back(normalIndex[i]);
					}				
				}
			}
			else
			{
				string temp;
				getline(myfile, temp);
			}
			count++;
        }
		cout << "Count: " << path << count << endl;
        myfile.close();
	}
    else
        cout << "Unable to open file";

	// For each vertex of each triangle
	for( int i=0; i<vertexIndices.size(); i++ ){

		// Get the indices of its attributes
		int vertexIndex = vertexIndices[i];
		int uvIndex = uvIndices[i];
		int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		//glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec2 uv = glm::vec2(0.0f, 0.0f);
		glm::vec3 normal = temp_normals[ normalIndex-1 ];
		
		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);
	
	}

	return true;
}


