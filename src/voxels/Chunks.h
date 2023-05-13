#pragma once
#include <glm/glm.hpp>

class Chunk;
class voxel;

class Chunks {
public:
	Chunk** chunks;
	size_t volume;
	unsigned int w, h, d;

	Chunks(int w, int h, int d);
	~Chunks();


	voxel* get(int x, int y, int z);
	Chunk* getChunk(int x, int y, int z);
	void set(int x, int y, int z, int id);
	voxel* rayCast(glm::vec3 a, glm::vec3 dir, float maxDist, glm::vec3& end, glm::vec3& norm, glm::vec3& iend);
}; 