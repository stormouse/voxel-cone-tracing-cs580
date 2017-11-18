#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

///Represents a transform: rotation, position and scale.
class Transform {
public:
	Transform();
	~Transform() {}

	// Recalculates the transform matrix according to the position, scale and rotation vectors. 
	void updateTransformMatrix();

	// Returns a reference to the transform matrix 
	glm::mat4 & getTransformMatrix();

	// Bunch of helper functions.
	glm::vec3 forward();
	glm::vec3 up();
	glm::vec3 right();

public:
	glm::vec3 position = glm::vec3(0), scale = glm::vec3(1), rotation = glm::vec3(0);
	// Is true when the transform matrix is not correctly representing the position, scale and rotation vectors. 
	bool transformIsInvalid = false;
private:
	glm::mat4 transform;
};