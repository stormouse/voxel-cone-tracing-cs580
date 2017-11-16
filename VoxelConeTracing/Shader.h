#pragma once

#include "glew.h"

class Shader
{
public:
	GLuint Program;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void Use();
	~Shader();
};

