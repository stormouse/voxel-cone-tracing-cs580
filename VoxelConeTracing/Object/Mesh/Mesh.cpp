#include "Mesh.h"

Mesh::Mesh() {
	this->staticDraw = true;
	this->meshUploaded = false;
}

Mesh::~Mesh() {
	if (this->meshUploaded) {
		GLint current_program;
		glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);

		// Delete.
		glUseProgram(programHandler);
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);

		// Reset.
		glUseProgram(current_program);
	}
}

void Mesh::BindMeshToBuffer() {
	if (this->meshUploaded)
		return;
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Bind vertex data
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(VertexData),
		vertexData.data(), staticDraw ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

	//bind vertex index
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *sizeof(unsigned int),
		indices.data(), staticDraw ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
	this->meshUploaded = true;
}

void Mesh::BindMeshToProgram(GLuint program, bool useProgram) {
	this->programHandler = program;
	if (useProgram) 
		glUseProgram(program);

	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	GLuint loc1 = glGetAttribLocation(programHandler, positionName);
	glEnableVertexAttribArray(loc1);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);

	GLuint loc2 = glGetAttribLocation(programHandler, normalName);
	glEnableVertexAttribArray(loc2);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, VertexData::normal));

	glUniformMatrix4fv(glGetUniformLocation(programHandler, modelMatrixName), 1, GL_FALSE, glm::value_ptr(transform.getTransformMatrix()));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
	
	glDisableVertexAttribArray(loc1);
	glDisableVertexAttribArray(loc2);
	glBindVertexArray(0);
}

