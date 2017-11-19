#include "Application.h"

#include "CornellScene.h"

Application::Application()
{
	this->scene = dynamic_cast<Scene*>(new CornellScene());
	this->voxelDimensions = 64;

	// load shader
	char* vvs = "Shaders/voxelization.vert";
	char* vgs = "Shaders/voxelization.geom";
	char* vfs = "Shaders/voxelization.frag";

	voxelizationShader = new Shader(vvs, vgs, vfs);

	// init texture
	glGenTextures(1, &voxelTexture3D);
	glBindTexture(GL_TEXTURE_3D, voxelTexture3D);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int numVoxels = voxelDimensions * voxelDimensions * voxelDimensions;
	GLubyte* data = new GLubyte[numVoxels * 4];
	memset(data, 0, numVoxels * 4 * sizeof(GLubyte));

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, voxelDimensions, voxelDimensions, voxelDimensions, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	delete[] data;

	glGenerateMipmap(GL_TEXTURE_3D);

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
	
	float size = scene->worldSize;

	auto projectionMatrix = glm::ortho(-size * 0.5f, size * 0.5f, -size*0.5f, size*0.5f, size * 0.5f, size * 1.5f);
	auto projX = projectionMatrix * glm::lookAt(glm::vec3(size, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	auto projY = projectionMatrix * glm::lookAt(glm::vec3(0, size, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
	auto projZ = projectionMatrix * glm::lookAt(glm::vec3(0, 0, size), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	
	glViewport(0, 0, voxelDimensions, voxelDimensions);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	voxelizationShader->Use();

	glUniform1i(glGetUniformLocation(voxelizationShader->Program, "voxelDimensions"), voxelDimensions);
	glUniformMatrix4fv(glGetUniformLocation(voxelizationShader->Program, "ProjX"), 1, GL_FALSE, &projX[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(voxelizationShader->Program, "ProjY"), 1, GL_FALSE, &projY[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(voxelizationShader->Program, "ProjZ"), 1, GL_FALSE, &projZ[0][0]);


	glBindImageTexture(2, voxelTexture3D, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glUniform1i(glGetUniformLocation(voxelizationShader->Program, "voxelTexture"), 2);


	scene->Render();


	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_3D, voxelTexture3D);
	glGenerateMipmap(GL_TEXTURE_3D);

	glViewport(0, 0, screenWidth, screenHeight);

	glUseProgram(0);

}