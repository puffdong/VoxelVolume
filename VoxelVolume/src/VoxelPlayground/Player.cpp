#define _USE_MATH_DEFINES
#include "Player.h"

Player::Player(glm::vec3 startPos)
	: model("res/models/lowpolybird.obj"), texture("res/textures/nerfthisbird.tga"), position(startPos), velocity(0.f, 0.f, 0.f),
	isOnGround(true), pitch(0.f), yaw(0.f), roll(0.f)
{
	shader = new Shader("res/shaders/Player.shader");

}

void Player::tick(float delta, ButtonMap bm)
{
	move(bm, delta);
}




void Player::move(ButtonMap bm, float delta)
{
	glm::vec3 movement = glm::vec3(0.0);

	if (bm.W) {
		movement += glm::vec3(
			sin(yaw),
			0.0,
			cos(yaw));
	}
		
	if (bm.S) {
		movement -= glm::vec3(
			sin(yaw),
			0.0,
			cos(yaw));
		}
	
	if (bm.A)
		yaw += 1.5 * delta;

	if (bm.D)
		yaw -= 1.5 * delta;
		
	if (bm.Space)
		movement += glm::vec3(0.f, 1.0, 0.0);

	if (bm.Ctrl)
		movement += glm::vec3(0.f, -1.0, 0.0);

	position += movement * kMoveSpeed * delta;
}

void Player::draw(glm::mat4 proj, glm::mat4 view, glm::mat4 modelM)
{
	shader->Bind();
	texture.Bind(0);
	shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

	shader->SetUniformMat4("proj", proj);
	shader->SetUniformMat4("view", view);
	shader->SetUniformMat4("model", modelM);


	shader->SetUniform1i("u_Texture", 0);
	shader->SetUniform3f("sunDir", glm::vec3(1.f, 0.5f, 0.f));
	shader->SetUniform3f("sunColor", glm::vec3(1.f, 1.f, 1.f));

	model.render();
}

glm::vec3 Player::getPosition()
{
	return position;
}

glm::mat4 Player::getModelMatrix() {
	return glm::translate(
		glm::mat4(1.f), position)
		* glm::rotate(glm::mat4(1.f), yaw, glm::vec3(0.f, 1.f, 0.f))
		* glm::rotate(glm::mat4(1.f), 0.0f, glm::vec3(1.f, 0.f, 0.f))
		* glm::rotate(glm::mat4(1.f), 0.0f, glm::vec3(0.f, 0.f, 1.f))
		* glm::scale(glm::mat4(1.f), glm::vec3(7.f)
		);
}

float Player::getYaw()
{
	return yaw;
}

void Player::setPosition(glm::vec3 pos)
{
	position = pos;
}