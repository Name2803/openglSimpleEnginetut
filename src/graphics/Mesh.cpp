#include "Mesh.h"

Mesh::Mesh(const float* buffer, size_t vertices, const int* attrs) : vertices(vertices)
{
	
	vertiexSize = 0;
	for (int i = 0; attrs[i]; i++) {
		vertiexSize += attrs[i];
	}
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * vertices * vertiexSize, buffer, GL_STATIC_DRAW);

	int offset = 0;
	for (int i = 0; attrs[i]; i++) {
		int size = attrs[i];
		glVertexAttribPointer(i, attrs[i], GL_FLOAT, GL_FALSE, vertiexSize * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(i);
		offset += size;
	}

}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}


void Mesh::reload(const float* buffer, size_t vertices) {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * vertices * vertiexSize, buffer, GL_STATIC_DRAW);
	this->vertices = vertices;
}


void Mesh::draw(unsigned int primitive) {
	glBindVertexArray(VAO);
	glDrawArrays(primitive, 0, vertices);
	glBindVertexArray(0);
}