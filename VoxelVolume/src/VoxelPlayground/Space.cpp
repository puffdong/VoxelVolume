#include "Space.h"

#include <iostream>

Space::Space() {
	player = new Player(nullptr, glm::vec3(0.0, 0.0, 0.0));
}

void Space::tick(float tick, ButtonMap bm) {
	
}