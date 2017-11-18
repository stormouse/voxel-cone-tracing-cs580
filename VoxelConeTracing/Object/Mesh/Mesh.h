#pragma once
#ifndef OBJECT_MESH_H_
#define OBJECT_MESH_H_

#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "../Transform/Transform.h"
using namespace std;

namespace {
	const char * positionName = "position";
	const char * normalName = "normal";
	const char * colorName = "color";
	const char * texCoordName = "texCoord";
	const char * indicesName = "indices";
	const char * modelMatrixName = "M";
}

/*
VertexData is a struct to store position, color, normal and texCoord of every vertex
*/
struct VertexData {
	glm::vec3 position, normal, color;
	glm::vec2 texCoord;

	VertexData(
		glm::vec3 _position = glm::vec3(0, 0, 0),
		glm::vec3 _normal = glm::vec3(0, 0, 0),
		glm::vec3 _color = glm::vec3(1, 1, 1),
		glm::vec2 _texCoord = glm::vec2(0, 0)) :
		position(_position),
		normal(_normal),
		color(_color),
		texCoord(_texCoord) {}
};

/*
CMesh is a class to store the VertexData, indices and transformation
*/
class Mesh {
public:
	Mesh();
	~Mesh();
	
	//bind vertex data and indices to buffer
	void BindMeshToBuffer();

	void BindMeshToProgram(GLuint program, bool useProgram = false);


public:
	vector<VertexData> vertexData;
	vector<unsigned int> indices;
	Transform transform;

	bool staticDraw;

	//vertex array object, vertex buffer object, element buffer object.
	GLuint vao, vbo, ebo;

private:
	GLuint programHandler;
	
	bool meshUploaded;
};


#endif // !OBJECT_MESH_H_
