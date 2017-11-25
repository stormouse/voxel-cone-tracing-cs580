#pragma once

#define GLEW_STATIC

#include "glew.h"
#include "glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Shader.h"
#include "Scene.h"
#include "VoxelVisualization.h"



class Application
{
public:
	Application();
	~Application();

	void GenerateVoxelMap();
	void renderConeTracing(RENDER_SCENE& scene, GLuint voxelTextureID);
	void renderConeTracing();

	int  Run();
	void Render();

	GLuint	voxelTexture3D;
private:
	
	int		voxelDimensions;
	int		screenWidth;
	int		screenHeight;

	Scene	*scene;
	Shader	*voxelizationShader;
	Shader	*VCTShader;

	GLFWwindow *window;

	int		InitGL();
};


#pragma region camera_movement_helpers
/* camera movement helpers */

static bool isKeyDown[256];
static double last_mouse_x = 400.0f, last_mouse_y = 300.0f;
static double curr_mouse_x, curr_mouse_y;

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	switch (key) {
	case GLFW_KEY_W:
	case GLFW_KEY_A:
	case GLFW_KEY_S:
	case GLFW_KEY_D:
		if (action == GLFW_PRESS) {
			isKeyDown[key] = true;
			// cout << key << " is down\n";
		}
		else if (action == GLFW_RELEASE) {
			isKeyDown[key] = false;
			// cout << key << " is up\n";
		}
	}
}

static void camera_movement_control(Scene *scene) {
	float h, v;
	h = v = 0.0f;
	if (isKeyDown[GLFW_KEY_A]) h -= 1.0f;
	if (isKeyDown[GLFW_KEY_D]) h += 1.0f;
	if (isKeyDown[GLFW_KEY_W]) v += 1.0f;
	if (isKeyDown[GLFW_KEY_S]) v -= 1.0f;

	scene->MoveCamera(h, v);
}


static void camera_rotation_control(Scene *scene) {
	float dx = curr_mouse_x - last_mouse_x,
		dy = curr_mouse_y - last_mouse_y;
	scene->RotateCamera(dx * 3e-3, dy * 2e-3);
}

#pragma endregion