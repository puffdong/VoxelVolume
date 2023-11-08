#pragma once

#include "../Shader.h"
#include "../Model.h"
#include <glm/glm.hpp>
#include "../VoxelUtils/ButtonMap.h"


class Player {
private:
	Model model;
	Shader* shader;
	//Texture texture;

	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 velocity;

public:
	Player(Shader* shader, glm::vec3 pos);

	void tick(float delta, ButtonMap bm);

	void draw();


};