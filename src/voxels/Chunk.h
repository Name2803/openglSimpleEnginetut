#pragma once

#define CHUNK_W 100
#define CHUNK_H 100
#define CHUNK_D 100
#define CHUNK_VOL (CHUNK_W * CHUNK_H * CHUNK_D)

class voxel;

class Chunk {
public:
	int x, y, z;
	bool modified = true;
	voxel* voxels;
	Chunk(int xposs, int ypos, int zpos);
	~Chunk();
};