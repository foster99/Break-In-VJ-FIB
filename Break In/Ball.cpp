#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Ball.h"
#include "StaticTileMap.h"

void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{	
	sizeBall = 9;
	speed = 2;
	spdModifierX = 1;
	spdModifierY = 1;

	tileSize = bank->getStaticMap()->getTileSize();

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
	bool collisionY = false;
	bool collisionX = false;

	     if (collisionY = (direction.y > 0 && collisionMoveDown()))		direction.y = -1;
	else if (collisionY = (direction.y < 0 && collisionMoveUp())  )		direction.y =  1;

	     if (collisionX = (direction.x > 0 && collisionMoveRight()))	direction.x = -1;
	else if (collisionX = (direction.x < 0 && collisionMoveLeft()) )	direction.x =  1;
	
	if (collisionX || collisionY) Game::instance().playBallSound();

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

void Ball::render()
{
	sprite->render();
}

void Ball::setBank(Bank* b)
{
	bank = b;
}

void Ball::setPosition(const glm::vec2& pos)
{
	posBall = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

bool Ball::collisionMoveLeft()
{
	int i0, i1, currSpeed;

	i0 = (posBall.y + 2) / tileSize;
	i1 = (posBall.y + sizeBall - 3) / tileSize;
	currSpeed = speed * spdModifierX;

	for (int s = 1; s <= speed; ++s) {
		for (int i = i0; i <= i1; i++)
		{
			if (bank->tileIsSolid(i, (posBall.x - s) / tileSize))
			{
				posBall.x -= s - 1;
				return true;
			}
		}
	}

	posBall.x -= speed;
	return false;
}

bool Ball::collisionMoveRight()
{
	int i0, i1, currSpeed;

	i0 = (posBall.y + 2) / tileSize;
	i1 = (posBall.y + sizeBall - 3) / tileSize;
	currSpeed = speed * spdModifierX;

	for (int s = 1; s <= speed; ++s) {
		for (int i = i0; i <= i1; i++)
		{
			if (bank->tileIsSolid(i, (posBall.x + sizeBall + s) / tileSize))
			{
				posBall.x += s - 1;
				return true;
			}
		}
	}

	posBall.x += speed;
	return false;
}

bool Ball::collisionMoveDown()
{
	int j0, j1, currSpeed;

	j0 = (posBall.x + 2) / tileSize;
	j1 = (posBall.x + sizeBall - 3) / tileSize;
	currSpeed = speed * spdModifierY;

	for (int s = 1; s <= speed; ++s) {
		for (int j = j0; j <= j1; j++)
		{
			if (bank->tileIsSolid((posBall.y + sizeBall + s) / tileSize, j))
			{
				posBall.y += s - 1;
				return true;
			}
		}
	}

	posBall.y += speed;
	return false;
}

bool Ball::collisionMoveUp()
{
	int j0, j1, currSpeed;

	j0 = (posBall.x + 2) / tileSize;
	j1 = (posBall.x + sizeBall - 3) / tileSize;
	currSpeed = speed * spdModifierY;

	for (int s = 1; s <= speed; ++s) {

		for (int j = j0; j <= j1; j++)
		{
			if (bank->tileIsSolid((posBall.y - s) / tileSize, j))
			{
				posBall.y -= float(s - 1);
				return true;
			}
		}
	}

	posBall.y -= speed;
	return false;
}
