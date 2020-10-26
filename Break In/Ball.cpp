#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "Ball.h"

void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{	
	sizeBall = 9;
	speed = 5;
	spdModifierX = 0.5;
	spdModifierY = 0.7;

	tex.loadFromFile("images/ball.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(sizeBall, sizeBall), glm::vec2(1.f, 1.f), &tex, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);

	direction.x = -1;
	direction.y = 1;

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
	
}

void Ball::update(int deltaTime)
{
	bool collision = false;

	if (collision = (direction.y > 0 && map->collisionMoveDown(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.y, int(speed * spdModifierY))))
		direction.y = -1;
	else if (collision = (direction.y < 0 && map->collisionMoveUp(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.y, int(speed * spdModifierY))))
		direction.y = 1;

	if (collision = (direction.x > 0 && map->collisionMoveRight(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.x, int(speed * spdModifierX))))
		direction.x = -1;
	else if (collision = (direction.x < 0 && map->collisionMoveLeft(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.x, int(speed * spdModifierX))))
		direction.x = 1;
	
	//if (collision) Game::instance().playBallSound();

	//posBall.x += speed * spdModifierX * direction.x;
	//posBall.y += speed * spdModifierY * direction.y;



	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

void Ball::render()
{
	sprite->render();
}

void Ball::changeModifierX(float value)
{
	spdModifierX = value;
}

void Ball::changeModifierY(float value)
{
	spdModifierY = value;
}

void Ball::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Ball::setPosition(const glm::vec2& pos)
{
	posBall = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}
