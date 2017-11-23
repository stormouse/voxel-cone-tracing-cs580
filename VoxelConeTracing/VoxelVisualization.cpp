#include "VoxelVisualization.h"


VoxelVisualization::VoxelVisualization(int width, int height) {
	this->width = width;
	this->height = height;

	voxelVisualizationShader = new Shader(voxelVisualizationShaderVS, voxelVisualizationShaderFS);
	worldPostionShader = new Shader(worldPositionShaderVS, worldPositionShaderFS);

	frontFBO = new FBO(width, height);
	backFBO = new FBO(width, height);

	cube = new Object("Assets\\Models\\cube.obj");
	quad = new Object("Assets\\Models\\quad.obj");
}

VoxelVisualization::~VoxelVisualization() {
	if (frontFBO) {
		delete frontFBO;
		frontFBO = NULL;
	}
	if (backFBO) {
		delete backFBO;
		backFBO = NULL;
	}
	if (worldPostionShader) {
		delete worldPostionShader;
		worldPostionShader = NULL;
	}
	if (voxelVisualizationShader) {
		delete voxelVisualizationShader;
		voxelVisualizationShader = NULL;
	}
	if (cube) {
		delete cube;
		cube = NULL;
	}
	if (quad) {
		delete quad;
		quad = NULL;
	}
}

void VoxelVisualization::RenderVoxelVisualization(RENDER_SCENE& scene, GLuint voxelTextureID) {
	GLuint worldPostionProgram = worldPostionShader->Program;
	glUseProgram(worldPostionProgram);

	scene.BindCameraToProgram(worldPostionProgram);

	glClearColor(0, 0, 0, 1);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glCullFace(GL_FRONT);
	glBindFramebuffer(GL_FRAMEBUFFER, backFBO->frameBuffer);
	glViewport(0, 0, backFBO->width, backFBO->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cube->Render(worldPostionProgram);

	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, frontFBO->frameBuffer);
	glViewport(0, 0, frontFBO->width, frontFBO->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cube->Render(worldPostionProgram);
	
	glUseProgram(0);

	GLuint voxelVisualizationProgram = voxelVisualizationShader->Program;
	glUseProgram(voxelVisualizationProgram);
	scene.BindCameraToProgram(voxelVisualizationProgram);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, frontFBO->textureBuffer);
	glUniform1i(glGetUniformLocation(voxelVisualizationProgram, textureFrontName), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, backFBO->textureBuffer);
	glUniform1i(glGetUniformLocation(voxelVisualizationProgram, textureBackName), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_3D, voxelTextureID);
	glUniform1i(glGetUniformLocation(voxelVisualizationProgram, texture3DName), 2);

	glViewport(0, 0, this->width, this->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	quad->Render(voxelVisualizationProgram);
	glUseProgram(0);
}
