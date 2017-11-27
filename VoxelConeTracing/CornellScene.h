#pragma once

#include "Scene.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

#include "Object\Object Loader\Object.h"

struct PointLight {
	glm::vec3 position;
	glm::vec3 color;
	float	  intensity;
};

class CornellScene : public Scene
{
public:
	CornellScene();
	void Render() override;
	void Render(GLuint program) override;
	virtual ~CornellScene();

	void SetScreenSize(int w, int h) override;

	void MoveCamera(float h, float v) override;
	void RotateCamera(float x, float y) override;

	const float* getViewTransform() override;
	const float* getProjectionTransform() override;

	void BindCameraToProgram(GLuint program) override;

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


	PointLight	light;
	double		lightTimeArg;
	double		lastRenderTime;

	double		_getTimeMS();

// debug use
public:
	Object *object, *bunny, *susanne, *teapot;

};

