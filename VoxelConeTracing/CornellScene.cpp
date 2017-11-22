#include "CornellScene.h"
#include <chrono>
#include <stdint.h>
#include <iostream>

CornellScene::CornellScene()
{
	this->screenWidth = 800;
	this->screenHeight = 600;

	this->cameraPosition = glm::vec3(0.0f, 0.0f, -10.0f);
	this->cameraMoveSpeed = 0.02f;

	this->cameraProjection = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 30.0f);
	
	this->angleX = 0.0f;
	this->angleY = 0.0f;

	this->cameraRotateX = glm::rotate(glm::mat4(1.0f), angleX, glm::vec3(0.0f, 1.0f, 0.0f));
	this->cameraRotateY = glm::rotate(glm::mat4(1.0f), -angleY, glm::vec3(1.0f, 0.0f, 0.0f));

	this->cameraForward = glm::vec3(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f) * cameraRotateY * cameraRotateX);
	this->cameraRight = glm::vec3(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f) * cameraRotateY * cameraRotateX);

	object = new Object("Assets\\Models\\cornell.obj");

}


CornellScene::~CornellScene()
{
	delete object;
}


void CornellScene::SetScreenSize(int w, int h) {
	assert(w > 100 && h > 100);
	this->screenWidth = w;
	this->screenHeight = h;
	this->cameraProjection = glm::perspective(
		45.0f, (float)w / (float)h, 0.1f, 30.0f);
}


void CornellScene::MoveCamera(float h, float v) {
	float l = cameraMoveSpeed / sqrt(h*h + v*v + 0.01f);
	h *= l; v *= l;
	this->cameraPosition -= cameraRight * h + cameraForward * v;
}


void CornellScene::RotateCamera(float dx, float dy) {
	this->angleX += dx;
	this->angleY += dy;
	if (angleY > 80.0f) angleY = 80.0f;
	if (angleY < -80.0f) angleY = -80.0f;
	cameraRotateX = glm::rotate(glm::mat4(1.0f), angleX, glm::vec3(0.0f, 1.0f, 0.0f));
	cameraRotateY = glm::rotate(glm::mat4(1.0f), angleY, glm::vec3(1.0f, 0.0f, 0.0f));
	this->cameraForward	= glm::vec3(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f) * cameraRotateY * cameraRotateX);
	this->cameraRight	= glm::vec3(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f) * cameraRotateY * cameraRotateX);
}


inline float CornellScene::_getTimeMS() {
	using namespace std::chrono;
	milliseconds ms = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch());
	int64_t now = ms.count();
	return (float)now * 0.001f;
}

void CornellScene::Render() {
	float now = _getTimeMS();
	float deltaTime = now - lastRenderTime;
	lastRenderTime = now;

	glm::mat4 modelMat = glm::mat4(1.0f);

	debug_testModel.Draw();
	// object->UploadRenderSetting();
}

void CornellScene::Render(GLuint program) {
	object->Render(program);
}

const float* CornellScene::getViewTransform() {
	glm::mat4 pos = glm::translate(glm::mat4(1.0f), cameraPosition);
	this->cameraView = cameraRotateY * cameraRotateX * pos;
	return glm::value_ptr(this->cameraView);
}

const float* CornellScene::getProjectionTransform() {
	return glm::value_ptr(cameraProjection);
}

void CornellScene::BindCameraToProgram(GLuint program) {
	glUniformMatrix4fv(glGetUniformLocation(program, viewMatrixName), 1, GL_FALSE, getViewTransform());
	glUniformMatrix4fv(glGetUniformLocation(program, projectionMatrixName), 1, GL_FALSE, getProjectionTransform());
	glUniform3fv(glGetUniformLocation(program, cameraPostionName), 1, glm::value_ptr(this->cameraPosition));
}


