#pragma once

#include "glew.h"
#include "glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Shader.h"
#include "Scene.h"
#include "VoxelVisualization.h"

struct PointLight {
	glm::vec3 position;
	glm::vec3 color;
	float	  intensity;
};

class Application
{
public:
	Application();
	~Application();

	void GenerateVoxelMap();
	void renderConeTracing(RENDER_SCENE& scene, GLuint voxelTextureID);
	void renderConeTracing();

	GLuint	voxelTexture3D;
private:
	
	int		voxelDimensions;

	int		screenWidth;
	int		screenHeight;

	//point light in cornellScene
	PointLight light;

	Scene	*scene;
	Shader	*voxelizationShader;
	Shader	*VCTShader;

};

