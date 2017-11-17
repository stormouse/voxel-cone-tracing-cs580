#include "Material.h"

void Material::BindMaterialToProgram(GLuint program, bool useProgram) {
	if (useProgram) glUseProgram(program);

	glUniform3fv(glGetUniformLocation(program, diffuseColorName), 1, glm::value_ptr(diffuseColor));
	glUniform3fv(glGetUniformLocation(program, specularColorName), 1, glm::value_ptr(specularColor));

	glUniform1f(glGetUniformLocation(program, emissivityName), emissivity);
	glUniform1f(glGetUniformLocation(program, specularReflectanceName), specularReflectivity);
	glUniform1f(glGetUniformLocation(program, diffuseReflectanceName), diffuseReflectivity);
	glUniform1f(glGetUniformLocation(program, specularDiffusionName), specularDiffusion);
	glUniform1f(glGetUniformLocation(program, transparencyName), transparency);
	glUniform1f(glGetUniformLocation(program, refractiveIndexName), refractiveIndex);

}