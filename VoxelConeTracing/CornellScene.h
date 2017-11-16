#pragma once

#include "Scene.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>


// debug model
#include "MockModel.h"

class CornellScene : Scene
{
public:
	CornellScene();
	void Render() override;
	virtual ~CornellScene();

	void SetScreenSize(int w, int h);

	void MoveCamera(float h, float v);
	void RotateCamera(float x, float y);

	const float* getViewTransform();
	const float* getProjectionTransform();

private:
	int			screenWidth;
	int			screenHeight;
	
	float		cameraMoveSpeed;
	float		angleX;
	float		angleY;

	glm::vec3	cameraPosition;
	glm::vec3	cameraForward;
	glm::vec3	cameraRight;
	glm::mat4	cameraProjection;
	glm::mat4	cameraView;
	glm::mat4	cameraRotateX;
	glm::mat4	cameraRotateY;

	std::vector<glm::vec3> pointLightSources;

	

	float		lastRenderTime;

	float		_getTimeMS();

// debug use
public:
	MockModel debug_testModel;

};

