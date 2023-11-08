#pragma once
#include <glm/glm.hpp>
#include "Player.h"


// Container for the objects and acts like the general web connecting the objects together

class Space {
private:
	Player* player;

public:
	Space();

	void tick(float tick, ButtonMap bm);

};