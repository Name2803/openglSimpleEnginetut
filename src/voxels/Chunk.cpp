#include "Chunk.h"
#include "voxel.h"
#include <math.h>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

Chunk::Chunk(int xpos, int ypos, int zpos):
	x(xpos), y(ypos), z(zpos)
{
	voxels = new voxel[CHUNK_VOL];
	for (int z = 0; z < CHUNK_D; z++) {
		for (int x = 0; x < CHUNK_W; x++) {
				int real_x = x + this->x * CHUNK_W;
				int real_z = z + this->z * CHUNK_D;
				float height = glm::perlin(glm::vec3(x * 0.05f, z * 0.05f, 3));
			for (int y = 0; y < CHUNK_H; y++) {
				int real_y = y + this->y * CHUNK_H;
				int id = y <= (height) * 10;
				//int id = y <=  / 16;
				//if (real_y <= 2)
				//	id = 2;
				voxels[(y * CHUNK_D + z) * CHUNK_W + x].id = id;
			}
		}
	}
}

Chunk::~Chunk() {
	delete[] voxels;
}