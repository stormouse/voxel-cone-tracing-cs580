
#define GLEW_STATIC

#include "glew.h"
#include "glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Object\Object Loader\Object.h"

#include <iostream>
using namespace std;


void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
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

	// test glm successfully imported
	glm::mat4 check_glm_imported = glm::rotate(glm::mat4(1.0f), 45.0f,
		glm::vec3(0, 1.0f, 0));

	//Object object = Object("Assets\\Models\\bunny.obj");
	//Object object = Object(); //default is Assets\\Models\\cornell.obj
	//GLuint program = 0;
	//object.UploadRenderSetting(program);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.4f, 0.5f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
	}

	return 0;
}

