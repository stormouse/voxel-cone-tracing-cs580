#pragma once

#include "glew.h"

class Shader
{
public:
	GLuint Program;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	Shader(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath);
	void Use();
	~Shader();
private:
	enum ShaderPipelineType { VertFrag, VertGeomFrag };
	ShaderPipelineType pipelineType;
};

