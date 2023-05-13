#pragma once 

#include <stdlib.h>
#include <glad/glad.h>
#include <iostream>

class Mesh {
	GLuint VAO, VBO;
	size_t vertices;
	float* buffer;
	size_t vertiexSize;
public:
	Mesh(const float* buffer, size_t vertices, const int* attrs);
	~Mesh();

	void reload(const float* buffer, size_t vertices);
	void draw(unsigned int primitive);

};