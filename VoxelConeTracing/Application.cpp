#include "Application.h"

#include "CornellScene.h"
#include "namespace.h"



Application::Application()
{
	this->scene = new CornellScene();
	this->voxelDimensions = 64;

	//light.color = glm::vec3(1.0f, 1.0f, 1.0f);
	light.color = glm::vec3(1.4f, 0.9f, 0.35f);
	light.color = glm::normalize(light.color);
	light.intensity = 2.3f;
	light.position = glm::vec3(-0.2f, 0.6f, 0.6f);

	// load shader
	char* vvs = "Shaders/voxelization.vert";
	char* vgs = "Shaders/voxelization.geom";
	char* vfs = "Shaders/voxelization.frag";

	voxelizationShader = new Shader(vvs, vgs, vfs);


	//load voxel cone tracing shader
	char* vctV = "Shaders/cone_tracing.vert";
	char* vctF = "Shaders/cone_tracing.frag";

	VCTShader = new Shader(vctV, vctF);

	// init texture
	glGenTextures(1, &voxelTexture3D);
	glBindTexture(GL_TEXTURE_3D, voxelTexture3D);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int numVoxels = voxelDimensions * voxelDimensions * voxelDimensions;
	GLfloat* data = new GLfloat[numVoxels * 4];
	memset(data, 0, numVoxels * 4 * sizeof(GLfloat));

	glTexStorage3D(GL_TEXTURE_3D, 7, GL_RGBA8, voxelDimensions, voxelDimensions, voxelDimensions);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, voxelDimensions, voxelDimensions, voxelDimensions, 0, GL_RGBA, GL_FLOAT, data);
	delete[] data;

	glGenerateMipmap(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, 0);
}


Application::~Application()
{
	if (this->scene != nullptr)
		delete this->scene;

	if(this->voxelTexture3D != 0)
		glDeleteTextures(1, &voxelTexture3D);

	if (this->voxelizationShader != nullptr)
		delete this->voxelizationShader;
}


void Application::GenerateVoxelMap() {
	//float clearColor[4] = { 1, 1, 1, 1 };
	//GLint previousBoundTextureID;
	//glGetIntegerv(GL_TEXTURE_BINDING_3D, &previousBoundTextureID);
	//glBindTexture(GL_TEXTURE_3D, voxelTexture3D);
	//glClearTexImage(voxelTexture3D, 0, GL_RGBA, GL_FLOAT, &clearColor);
	//glBindTexture(GL_TEXTURE_3D, previousBoundTextureID);

	voxelizationShader->Use();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glViewport(0, 0, voxelDimensions, voxelDimensions);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform1i(glGetUniformLocation(voxelizationShader->Program, voxelDimensionName), voxelDimensions);
	glUniformMatrix4fv(glGetUniformLocation(voxelizationShader->Program, viewMatrixName), 1, GL_FALSE, scene->getViewTransform());
	glUniformMatrix4fv(glGetUniformLocation(voxelizationShader->Program, projectionMatrixName), 1, GL_FALSE, scene->getProjectionTransform());
	glUniform3fv(glGetUniformLocation(voxelizationShader->Program, lightPositionName), 1, glm::value_ptr(light.position));
	glUniform3fv(glGetUniformLocation(voxelizationShader->Program, lightColorName), 1, glm::value_ptr(light.color));
	glUniform1f(glGetUniformLocation(voxelizationShader->Program, lightIntensityName), light.intensity);


	int texIdx = 0;
	glActiveTexture(GL_TEXTURE0 + texIdx);
	glBindTexture(GL_TEXTURE_3D, voxelTexture3D);
	glUniform1i(glGetUniformLocation(voxelizationShader->Program, texture3DName), texIdx);
	glBindImageTexture(texIdx, voxelTexture3D, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);

	scene->Render(voxelizationShader->Program);

	glGenerateMipmap(GL_TEXTURE_3D);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	//glViewport(0, 0, screenWidth, screenHeight);

	glUseProgram(0);

}

void Application::renderConeTracing(RENDER_SCENE& scene, GLuint voxelTextureID) {
	//Fetch reference
	const GLuint curProgram = VCTShader->Program;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(curProgram);

	//gl setting
	int viewportWidth = 800;
	int viewportHeight = 600;
	//glfwGetWindowSize(currentWindow, &viewportWidth, &viewportHeight);
	glViewport(0, 0, viewportWidth, viewportHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//upload camera info to shader
	//this->scene->BindCameraToProgram(curProgram);
	scene.BindCameraToProgram(curProgram);
	//upload voxel info
	glUniform1i(glGetUniformLocation(curProgram, voxelDimensionName), voxelDimensions);
	//upload transform matrix
	//glUniformMatrix4fv(glGetUniformLocation(curProgram, viewMatrixName), 1, GL_FALSE, this->scene->getViewTransform());
	//glUniformMatrix4fv(glGetUniformLocation(curProgram, projectionMatrixName), 1, GL_FALSE, this->scene->getProjectionTransform());
	glUniformMatrix4fv(glGetUniformLocation(curProgram, viewMatrixName), 1, GL_FALSE, scene.getViewTransform());
	glUniformMatrix4fv(glGetUniformLocation(curProgram, projectionMatrixName), 1, GL_FALSE, scene.getViewTransform());
	//upload light info to shader
	glUniform3fv(glGetUniformLocation(curProgram, lightPositionName), 1, glm::value_ptr(light.position));
	glUniform3fv(glGetUniformLocation(curProgram, lightColorName), 1, glm::value_ptr(light.color));
	glUniform1f(glGetUniformLocation(curProgram, lightIntensityName), light.intensity);
	//active the texture unit
	glActiveTexture(GL_TEXTURE3);
	//glBindTexture(GL_TEXTURE_3D, voxelTexture3D);
	glBindTexture(GL_TEXTURE_3D, voxelTextureID);
	glUniform1i(glGetUniformLocation(curProgram, texture3DName), 3);
	//this->scene->Render(curProgram);
	scene.Render(curProgram);
}

void Application::renderConeTracing() {
	//Fetch reference
	const GLuint curProgram = VCTShader->Program;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(curProgram);

	//for test
	light.color = glm::vec3(1.0, 1.0, 1.0);

	//upload camera info to shader
	this->scene->BindCameraToProgram(curProgram);
	//upload voxel info
	glUniform1i(glGetUniformLocation(curProgram, voxelDimensionName), voxelDimensions);
	//upload transform matrix
	glUniformMatrix4fv(glGetUniformLocation(curProgram, viewMatrixName), 1, GL_FALSE, this->scene->getViewTransform());
	glUniformMatrix4fv(glGetUniformLocation(curProgram, projectionMatrixName), 1, GL_FALSE, this->scene->getProjectionTransform());
	//upload light info to shader
	glUniform3fv(glGetUniformLocation(curProgram, lightPositionName), 1, glm::value_ptr(light.position));
	glUniform3fv(glGetUniformLocation(curProgram, lightColorName), 1, glm::value_ptr(light.color));
	glUniform1f(glGetUniformLocation(curProgram, lightIntensityName), light.intensity);
	//active the texture unit
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_3D, voxelTexture3D);
	glUniform1i(glGetUniformLocation(curProgram, texture3DName), 3);

	//gl setting
	int viewportWidth = 800;
	int viewportHeight = 600;
	//glfwGetWindowSize(currentWindow, &viewportWidth, &viewportHeight);
	glViewport(0, 0, viewportWidth, viewportHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	this->scene->Render(curProgram);
}