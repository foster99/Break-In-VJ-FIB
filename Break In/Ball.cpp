#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "Ball.h"

void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{	
	sizeBall = 16;
	tex.loadFromFile("images/bee.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(sizeBall, sizeBall), glm::vec2(1.f, 1.f), &tex, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);

	direction.x = 1;
	direction.y = 1;

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
	
}

void Ball::update(int deltaTime)
{	
	if (direction.y > 0) {
		if (map->collisionMoveDown(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.y))
			direction.y = -1;
		if (map->collisionMoveLeft(posBall, glm::ivec2(sizeBall, sizeBall)))
			direction.x = 1;
		else if (map->collisionMoveRight(posBall, glm::ivec2(sizeBall, sizeBall)))
			direction.x = -1;
	}
	else {
		if (map->collisionMoveUp(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.y))
			direction.y = 1;
		if (map->collisionMoveLeft(posBall, glm::ivec2(sizeBall, sizeBall)))
			direction.x = 1;
		else if (map->collisionMoveRight(posBall, glm::ivec2(sizeBall, sizeBall)))
			direction.x = -1;
	}

	posBall.x += 2 * direction.x;
	posBall.y += 2 * direction.y;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

void Ball::render()
{
	sprite->render();
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
