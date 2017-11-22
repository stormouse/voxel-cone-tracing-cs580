#pragma once
#ifndef VOXEL_VISUALIZATION_H_
#define VOXEL_VISUALIZATION_H_

#include "glew.h"
#include "glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
using namespace std;

#include "Object\Object Loader\Object.h"
#include "namespace.h"
#include "Shader.h"
#include "Scene.h"
#include "CornellScene.h"

using RENDER_SCENE = CornellScene;

struct FBO {
	GLuint width, height;
	GLuint frameBuffer, textureBuffer, renderBuffer;
	FBO(GLuint w, GLuint h) : width(w), height(h) {
		GLint previousFrameBuffer;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer);

		// Init framebuffer.
		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		glGenTextures(1, &textureBuffer);
		glBindTexture(GL_TEXTURE_2D, textureBuffer);

		// Texture parameters.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0);

		glGenRenderbuffers(1, &renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			cerr << "FBO failed to initialize correctly." << endl;
		}
	}
};

class VoxelVisualization
{
public:
	VoxelVisualization(int width, int height);
	~VoxelVisualization();

	void RenderVoxelVisualization(RENDER_SCENE& scene, GLuint voxelTextureID);

private:
	

private:
	int width, height;
	FBO *frontFBO, *backFBO;
	Shader *worldPostionShader, *voxelVisualizationShader;
	Object *cube, *quad;

};




#endif // !VOXEL_VISUALIZATION_H_
