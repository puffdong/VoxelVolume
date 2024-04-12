#include "Space.h"
#include <iostream>

Space::Space()
	: perlinNoise("res/textures/perlinnoise.tga")
{

	player = new Player(glm::vec3(0.f, 0.f, 0.f));

	glm::vec3 cameraDir(0.f, 0.f, 1.f);
	camera = new Camera(cameraDir, player);

	loadLevel1();
}

void Space::tick(float delta, ButtonMap bm)
{
	if (deathTimer > 1.f) { // Upon Player death this is activated
		deathTimer -= delta;
	}
	else if (deathTimer > 0.f) {
		deathTimer -= delta;
		resetPlayer();
	}
	else {
		player->tick(delta, bm);
		glm::vec3 playerPos = player->getPosition();
		float groundHeightPlayer = ground->calcHeight(playerPos.x, playerPos.z);
		float distanceToGround = playerPos.y - groundHeightPlayer;
		if (groundHeightPlayer < -1.f || distanceToGround < 0.f) {
			deathTimer = 4.f;
		}
	}

	glm::vec3 cameraPos = camera->getPosition();
	float groundHeightCamera = ground->calcHeight(cameraPos.x, cameraPos.z);
	if (cameraPos.y - groundHeightCamera < 2.f) {
		bool succeeded;
		zoomOutTimer = 0.5f;
		// Zoom in until camera stops colliding
		do {
			succeeded = camera->zoomIn(0.1f);
			cameraPos = camera->getPosition();
			groundHeightCamera = ground->calcHeight(cameraPos.x, cameraPos.z);

		} while (succeeded && cameraPos.y - groundHeightCamera < 0.f);
	}
	else if (zoomOutTimer < 0.f) {
		// Try to zoom out if the timer has reached 0
		camera->zoomOut(0.1f);
	}
	else {
		zoomOutTimer -= delta;
	}

	camera->tick(delta, bm);
}



void Space::resetPlayer()
{
	player->setPosition(playerStartPos);
	player->yaw = 0.f;
	player->pitch = 0.f;
	player->roll = 0.f;
}



void Space::renderWorld(float delta)
{
	glm::mat4 viewMatrix = camera->getLookAt();
	skybox->draw(proj, camera);

	ground->draw(proj, viewMatrix);

	for (WorldObject* o : wObjects)
	{
		o->tick(delta);
		o->draw(proj, viewMatrix, o->getModelMatrix());
	}

	/*for (Coin* c : coins)
	{
		c->draw(proj, viewMatrix, c->getModelMatrix());
	}*/

	player->draw(proj, viewMatrix, player->getModelMatrix());

	for (WorldObject* o : wTransparentObjects) {
		o->tick(delta);
		o->draw(proj, viewMatrix, o->getModelMatrix());
	}
}



void Space::loadLevel1()
{
	skybox = new Skybox(
		std::string("res/models/skybox-full-tweaked.obj"),
		std::string("res/shaders/Skybox.shader"),
		std::string("res/textures/skybox/cloud-landscape.tga")
	);

	playerStartPos = glm::vec3(30.f, 40.f, 0.f);
	player->setPosition(playerStartPos);

	// Setup shader with lighting
	Shader* worldShader = new Shader("res/shaders/WorldObject.shader");
	LightSource newLightSources[] = {
		//		LightSource(glm::vec3(0.f, 0.5f, 0.f), glm::vec3(-1.f, 0.f, 0.f), true),
				LightSource(glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 0.f), true)
	};

	std::vector<glm::vec3> lightColors;
	std::vector<glm::vec3> lightDirs;
	std::vector<int> isDirectional;
	for (LightSource& light : newLightSources)
	{
		lightColors.push_back(light.color);
		lightDirs.push_back(light.dir);
		isDirectional.push_back((int)light.isDirectional);
	}
	worldShader->Bind();
	worldShader->SetUniform1i("numLights", lightColors.size());
	worldShader->SetUniform3fv("lightColors", lightColors);
	worldShader->SetUniform3fv("lightDirs", lightDirs);
	worldShader->SetUniform1iv("isDirectional", isDirectional);

	// load all the world objects and set up the world
	WorldObject* teapotObject = new WorldObject(worldShader, "res/models/teapot.obj", glm::vec3(-10.f, 0.f, 0.f), glm::vec3(0.f));
	wObjects.push_back(teapotObject);

	glm::vec3 groundDims(100.f, 20.f, 100.f);
	glm::mat4 groundTrans = glm::translate(glm::mat4(1.f), glm::vec3(0.f));
	ground = new Ground(
		groundDims, groundTrans, "res/textures/terrain/fft-terrain.tga", worldShader, "res/textures/grass.tga"
	);


	//water stuff
	waterShader = new Shader("res/shaders/Water.shader");
	waterModel = new ModelObject(80.f, 80.f, 80, 80);
	WorldObject* water = new WaterSurface(waterShader, waterModel, glm::vec3(50.f, 5.f, 60.f), glm::vec3(0.f), &perlinNoise, player);
	wTransparentObjects.push_back(water);

	
}




