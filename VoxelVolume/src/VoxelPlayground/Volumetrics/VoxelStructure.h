#pragma once
#include "glm/glm.hpp"
#include <vector>
#include "../../OBJLoader.h"
#include "../../Shader.h" 

class VoxelStructure {
private:
	// Defines the height, width and depth. 
	int height;
	int width;
	int depth;
	
	glm::ivec3 position; // Where it is placed in WorldSpace

	std::vector<int> voxels;
	int length; // 3D squashed to 1D array
	float cellSize; // The size it takes of it in the world

	Shader* shader; // the debugging voxel viewqa
	ModelObject* voxelCube; // used for representing a voxel


public:
	VoxelStructure(int h, int w, int d, glm::ivec3 pos, int initValue, float cellSize);

	void setVoxelValue(int x, int y, int z, int value);
	int getVoxelValue(int x, int y, int z);

	//glm::ivec3 getWorldSpaceToVoxel(glm::vec3 worldVec);

	void drawVoxels(glm::vec3 projMatrix);

};