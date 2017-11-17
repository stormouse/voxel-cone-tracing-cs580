#pragma once
#ifndef OBJECT_MATERIAL_H_
#define OBJECT_MATERIAL_H_

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

namespace {
	const char * diffuseColorName = "diffuseColor";
	const char * specularColorName = "specularColor";
	const char * emissivityName = "emissivity";
	const char * transparencyName = "transparency";
	const char * refractiveIndexName = "refractiveIndex";
	const char * specularReflectanceName = "specularReflectivity";
	const char * diffuseReflectanceName = "diffuseReflectivity";
	const char * specularDiffusionName = "specularDiffusion";
}

struct Material {
	glm::vec3 diffuseColor, specularColor;
	float specularReflectivity, diffuseReflectivity, emissivity, specularDiffusion;
	float transparency, refractiveIndex;

	Material(
		glm::vec3 _diffuseColor = glm::vec3(1),
		float _emissivity = 0.0f,
		float _specularReflectivity = 0.0f,
		float _diffuseReflectivity = 1.0f,
		glm::vec3 _specularColor = glm::vec3(1),
		float _specularDiffusion = 2.0f,
		float _transparency = 0.0f,
		float _refractiveIndex = 1.4f
	):
		diffuseColor(_diffuseColor),
		specularColor(_specularColor),
		specularReflectivity(_specularReflectivity),
		diffuseReflectivity(_diffuseReflectivity),
		emissivity(_emissivity),
		specularDiffusion(_specularDiffusion),
		transparency(_transparency),
		refractiveIndex(_refractiveIndex){}

	static Material * Default() {
		return new Material();
	}

	static Material * White() {
		return new Material(
			glm::vec3(0.97f, 0.97f, 0.97f)
		);
	}

	static Material * Cyan() {
		return new Material(
			glm::vec3(0.30f, 0.95f, 0.93f)
		);
	}

	static Material * Purple() {
		return new Material(
			glm::vec3(0.97f, 0.05f, 0.93f)
		);
	}

	static Material * Red() {
		return new Material(
			glm::vec3(1.0f, 0.26f, 0.27f)
		);
	}

	static Material * Green() {
		return new Material(
			glm::vec3(0.27f, 1.0f, 0.26f)
		);
	}

	static Material * Blue() {
		return new Material(
			glm::vec3(0.35f, 0.38f, 1.0f)
		);
	}

	static Material * Emissive() {
		return new Material(
			glm::vec3(0.85f, 0.9f, 1.0f),
			1.0f
		);
	}

	//Bind Material to shader program
	void BindMaterialToProgram(GLuint program, bool useProgram = false);
};
#endif // !OBJECT_MATERIAL_H_

