#pragma once

class Scene
{
public:
	Scene();

	virtual void	Render() = 0;
	virtual void	Render(unsigned int program) = 0;
	virtual void	MoveCamera(float, float) = 0;
	virtual void	RotateCamera(float, float) = 0;
	virtual const float* getViewTransform() = 0;
	virtual const float* getProjectionTransform() = 0;
	virtual void    SetScreenSize(int w, int h) = 0;
	virtual void	BindCameraToProgram(unsigned int program) = 0;

	float worldSize = 2.0f;
	
	virtual ~Scene();
};

