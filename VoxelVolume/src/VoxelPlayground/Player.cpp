#pragma once

#include "Player.h"


Player::Player(Shader* shader, glm::vec3 pos)
	: pos(pos)
{
	rot = glm::vec3(0.0, 0.0, 0.0);
}

void Player::tick(float delta, ButtonMap bm) {

}

void Player::draw() {

}