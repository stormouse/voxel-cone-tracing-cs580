#include "Application.h"

#include "CornellScene.h"


struct PointLight {
	glm::vec3 position;
	glm::vec3 color;
	float	  intensity;
};


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

	PointLight light;
	light.color = glm::vec3(1.0f, 1.0f, 1.0f);
	light.intensity = 1.0f;
	light.position = glm::vec3(-0.2f, 0.8f, 0.0f);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	
	glViewport(0, 0, voxelDimensions, voxelDimensions);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	voxelizationShader->Use();

	glUniform1i(glGetUniformLocation(voxelizationShader->Program, "voxelDimensions"), voxelDimensions);
	glUniformMatrix4fv(glGetUniformLocation(voxelizationShader->Program, "V"), 1, GL_FALSE, scene->getViewTransform());
	glUniformMatrix4fv(glGetUniformLocation(voxelizationShader->Program, "P"), 1, GL_FALSE, scene->getProjectionTransform());
	glUniform3fv(glGetUniformLocation(voxelizationShader->Program, "pointlights[0].position"), 1, glm::value_ptr(light.position));
	glUniform3fv(glGetUniformLocation(voxelizationShader->Program, "pointlights[0].color"), 1, glm::value_ptr(light.color));
	glUniform1f(glGetUniformLocation(voxelizationShader->Program, "pointlights[0].intensity"), light.intensity);


	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_3D, voxelTexture3D);
	//glBindImageTexture(2, voxelTexture3D, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glUniform1i(glGetUniformLocation(voxelizationShader->Program, "voxelTexture"), 2);


	scene->Render(voxelizationShader->Program);

	glGenerateMipmap(GL_TEXTURE_3D);

	glViewport(0, 0, screenWidth, screenHeight);

	glUseProgram(0);

}