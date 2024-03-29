#include "LineBatch.h"
#include "Mesh.h"

#include <glad/glad.h>

#define LB_VERTEX_SIZE (3+4)

LineBatch::LineBatch(size_t capacity) : capacity(capacity), index(0) {
	int attrs[] = { 3, 4, 0 };
	this->attrs = attrs;
}

LineBatch::~LineBatch() {
	delete[] buffer;
	delete mesh;
}

void LineBatch::line(float x1, float y1, float z1, float x2, float y2, float z2,
	float r, float g, float b, float a) {

	buffer = new float[14];
	buffer[index] = x1;
	buffer[index + 1] = y1;
	buffer[index + 2] = z1;
	buffer[index + 3] = r;
	buffer[index + 4] = g;
	buffer[index + 5] = b;
	buffer[index + 6] = a;
	index += LB_VERTEX_SIZE;

	buffer[index] = x2;
	buffer[index + 1] = y2;
	buffer[index + 2] = z2;
	buffer[index + 3] = r;
	buffer[index + 4] = g;
	buffer[index + 5] = b;
	buffer[index + 6] = a;
	index += LB_VERTEX_SIZE;
	//return buffer;
}

void LineBatch::render() {
	if (index == 0)
		return;
	//mesh->reload(buffer, index / LB_VERTEX_SIZE, attrs);
	mesh->draw(GL_LINES);
	//index = 0;
}
