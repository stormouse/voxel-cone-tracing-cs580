#pragma once
#ifndef OBJECT_H_
#define OBJECT_H_

#define __UTILITY_LOG_LOADING_TIME true

#include <fstream>
#include <vector>
#include <algorithm>

#if __UTILITY_LOG_LOADING_TIME
#define GLEW_STATIC
#include <iostream>
#include <iomanip>
#include <glew.h>
#include <glfw3.h>
#endif

#include "tiny_obj_loader.h"
#include "../Mesh/Mesh.h"
#include "../Transform/Transform.h"
#include "../Material/Material.h"

using namespace std;

enum ObjectType
{
	Buddha,
	Bunny,
	Cornell,
	Dragon,
	Susanne,
	Quad,
	None
};


/*
This class is used for load object, helped by tiny_obj_loader.h.
Object is stored in an array of Mesh.
Each Mesh contains one object's position, color, normal, texCoord, which are stored in VertexData array,
and vertices' indices are stored in indices array.
*/
class Object
{
public:
	//load object when initialized an instance
	Object(string path = "Assets\\Models\\cornell.obj");
	~Object();

	void Render(GLuint program);

private:
	void CheckObjectPath();
	void createQuad();

	void LoadObject();
	void BindObjectToBuffer();

	void TransformObject();
	void TransformBuddha();
	void TransformBunny();
	void TransformCornell();
	void TransformDragon();	
	void TransformSusanne();

	void AssignMaterial();
	void BuddhaMaterial();
	void BunnyMaterial();
	void CornellMaterial();
	void DragonMaterial();
	void SusanneMaterial();


public:
	vector<Mesh> meshes;
	vector<Material*> materials;

private:
	string obj_path;
	int mesh_size;
	ObjectType objectType;
	bool has_material;
};


#endif // !OBJECT_H_
