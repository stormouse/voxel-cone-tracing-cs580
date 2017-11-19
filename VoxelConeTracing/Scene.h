#pragma once
class Scene
{
public:
	Scene();

	virtual void Render() = 0;
	
	float worldSize = 2.0f;
	
	~Scene();
};

