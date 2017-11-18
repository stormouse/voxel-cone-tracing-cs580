#include "Transform.h"

#include <ostream>

Transform::Transform() {
	updateTransformMatrix();
}

void Transform::updateTransformMatrix() {
	transform = glm::translate(position) * glm::mat4_cast(glm::quat(rotation)) * glm::scale(scale);
	transformIsInvalid = false;
}

glm::mat4 & Transform::getTransformMatrix() {
	if (transformIsInvalid) { updateTransformMatrix(); }
	return transform;
}

glm::vec3 Transform::forward() { 
	return glm::quat(rotation) * glm::vec3(0, 0, 1); 
}
glm::vec3 Transform::up() { 
	return glm::quat(rotation) * glm::vec3(0, 1, 0); 
}
glm::vec3 Transform::right() { 
	return glm::quat(rotation) * glm::vec3(-1, 0, 0); 
}
