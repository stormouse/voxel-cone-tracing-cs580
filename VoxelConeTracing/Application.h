#pragma once

#include "glew.h"
#include "glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Shader.h"
#include "Scene.h"


class Application
{
public:
	Application();
	~Application();

	void GenerateVoxelMap();


private:
	GLuint	voxelTexture3D;
	int		voxelDimensions;

	int		screenWidth;
	int		screenHeight;

	Scene	*scene;
	Shader	*voxelizationShader;

};

