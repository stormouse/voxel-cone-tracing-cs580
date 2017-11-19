
#define GLEW_STATIC

#include "glew.h"
#include "glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
using namespace std;

#include "Shader.h"
#include "CornellScene.h"
#include "Application.h"


void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

bool isKeyDown[256];
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
			cout << key << " is down\n";
		}
		else if (action == GLFW_RELEASE) {
			isKeyDown[key] = false;
			cout << key << " is up\n";
		}
	}
}



void camera_movement_control(CornellScene& scene) {
	float h, v;
	h = v = 0.0f;
	if (isKeyDown[GLFW_KEY_A]) h -= 1.0f;
	if (isKeyDown[GLFW_KEY_D]) h += 1.0f;
	if (isKeyDown[GLFW_KEY_W]) v += 1.0f;
	if (isKeyDown[GLFW_KEY_S]) v -= 1.0f;

	scene.MoveCamera(h, v);
}

double last_mouse_x = 400.0f, last_mouse_y = 300.0f;
double curr_mouse_x, curr_mouse_y;
void camera_rotation_control(CornellScene& scene) {
	float dx = curr_mouse_x - last_mouse_x,
		  dy = curr_mouse_y - last_mouse_y;
	scene.RotateCamera(dx * 1e-3, dy * 1e-3);
}


int main(int argc, char **argv) {
	if (!glfwInit()) {
		return EXIT_FAILURE;
	}

	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Empty Project", NULL, NULL);

	
	

	if (!window) {
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	glewInit();

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	char* basicShaderVS = "Shaders/basicshader.vert";
	char* basicShaderFS = "Shaders/basicshader.frag";
	Shader basicShader(basicShaderVS, basicShaderFS);


	CornellScene scene;
	Application app;
	cout << "Start generating voxel map" << endl;
	//app.GenerateVoxelMap();
	cout << "End generating voxel map" << endl;
	


	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glfwGetCursorPos(window, &curr_mouse_x, &curr_mouse_y);
		camera_rotation_control(scene);
		glfwSetCursorPos(window, 400, 300);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glClearColor(0.4f, 0.5f, 0.7f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		basicShader.Use();
		camera_movement_control(scene);

		glm::mat4 modelTransform = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(basicShader.Program, "M"),					 1, GL_FALSE, glm::value_ptr(modelTransform));
		glUniformMatrix4fv(glGetUniformLocation(basicShader.Program, "viewTransform"),		 1, GL_FALSE, scene.getViewTransform());
		glUniformMatrix4fv(glGetUniformLocation(basicShader.Program, "projectionTransform"), 1, GL_FALSE, scene.getProjectionTransform());

		
		scene.Render(basicShader.Program);
		//scene.Render();

		glfwSwapBuffers(window);
	}

	return 0;
}

