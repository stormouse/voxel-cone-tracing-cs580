#pragma once

#include "glew.h"
#include "glfw3.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include <vector>

class MockModel
{
public:
	MockModel();
	~MockModel();

	void Draw();

private:
	GLuint vbo;
};

