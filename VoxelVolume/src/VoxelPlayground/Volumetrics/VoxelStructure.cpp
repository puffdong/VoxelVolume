#include "VoxelStructure.h"

VoxelStructure::VoxelStructure(int h, int w, int d, glm::ivec3 pos, int initValue, float cellSize)
	: height(h), width(w), depth(d), position(pos), cellSize(cellSize)
{
	shader = new Shader("res/shaders/VoxelShaders/VoxelDebug.shader");

	voxels.resize(height * width * depth);
	length = height * width * depth;

	for (int i = 0; i < length; i++) {
		voxels[i] = initValue;
	}
}


void VoxelStructure::setVoxelValue(int x, int y, int z, int value) {
	int index = height * width * z + width * y + x;
	voxels[index] = value;
}

int VoxelStructure::getVoxelValue(int x, int y, int z) {
	int index = height * width * z + width * y + x;
	return voxels[index];
}

void VoxelStructure::drawVoxels(glm::vec3 projMatrix) {

}