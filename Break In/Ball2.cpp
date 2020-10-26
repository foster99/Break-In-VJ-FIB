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
	speed = 2;
	dirX = dirY = 1;

	dir = { 1.f, 1.f };
	dir = glm::normalize(dir);

	tex.loadFromFile("images/ball.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(sizeBall, sizeBall), glm::vec2(1.f, 1.f), &tex, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);

	direction.x = 1;
	direction.y = -1;

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
	
}

void Ball::update(int deltaTime)
{
	bool collision = false;

	glm::vec2 currPos, nextPos;

	currPos.x = posBall.x;
	currPos.y = posBall.y;

	nextPos.x = currPos.x + dirX * speed;
	nextPos.x = currPos.x + dirY * speed;


	if (dirX < 0.f) {	// Pelota sube
		
	}
	else {				// Pelota baja

	}


	     if (collision = (direction.y > 0 && map->collisionMoveDown(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.y, speed)))	direction.y = -1;
	else if (collision = (direction.y < 0 && map->collisionMoveUp(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.y,speed)))		direction.y = 1;
	else if (collision = (direction.x > 0 && map->collisionMoveRight(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.x, speed)))	direction.x = -1;
	else if (collision = (direction.x < 0 && map->collisionMoveLeft(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.x, speed)))	direction.x = 1;
	
	if (collision) Game::instance().playBallSound();

	posBall.x += speed * dirX * direction.x;
	posBall.y += speed * dirY * direction.y;



	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

vector<glm::vec2>  Ball::computePotentials(glm::vec2 p) {

	vector<glm::vec2> potentials;

	if (dir == glm::normalize(glm::vec2( 0.f, 4.f))) {
		potentials.resize(9,p);
		potentials[0] += glm::vec2( 0.f, 4.f);
		potentials[1] += glm::vec2(-1.f, 4.f);
		potentials[2] += glm::vec2( 1.f, 4.f);
		potentials[3] += glm::vec2(-2.f, 4.f);
		potentials[4] += glm::vec2( 2.f, 4.f);
		potentials[5] += glm::vec2(-3.f, 3.f);
		potentials[6] += glm::vec2( 3.f, 3.f);
		potentials[7] += glm::vec2(-4.f, 2.f);
		potentials[8] += glm::vec2( 4.f, 2.f);
	}
	else if (dir == glm::normalize(glm::vec2( 0.f,-4.f))) {
		potentials.resize(9);
		potentials[0] += glm::vec2( 0.f, -4.f);
		potentials[1] += glm::vec2(-1.f, -4.f);
		potentials[2] += glm::vec2( 1.f, -4.f);
		potentials[3] += glm::vec2(-2.f, -4.f);
		potentials[4] += glm::vec2( 2.f, -4.f);
		potentials[5] += glm::vec2(-3.f, -3.f);
		potentials[6] += glm::vec2( 3.f, -3.f);
		potentials[7] += glm::vec2(-4.f, -2.f);
		potentials[8] += glm::vec2( 4.f, -2.f);
	}
	else if (dir == glm::normalize(glm::vec2( 4.f, 0.f))) {
		potentials.resize(9);
		potentials[0] += glm::vec2(4.f, 0.f);
		potentials[1] += glm::vec2(4.f,-1.f);
		potentials[2] += glm::vec2(4.f, 1.f);
		potentials[3] += glm::vec2(4.f,-2.f);
		potentials[4] += glm::vec2(4.f, 2.f);
		potentials[5] += glm::vec2(3.f,-3.f);
		potentials[6] += glm::vec2(3.f, 3.f);
		potentials[7] += glm::vec2(2.f,-4.f);
		potentials[8] += glm::vec2(2.f, 4.f);
	}
	else if (dir == glm::normalize(glm::vec2(-4.f, 0.f))) {
		potentials.resize(9);
		potentials[0] += glm::vec2(-4.f, 0.f);
		potentials[1] += glm::vec2(-4.f,-1.f);
		potentials[2] += glm::vec2(-4.f, 1.f);
		potentials[3] += glm::vec2(-4.f,-2.f);
		potentials[4] += glm::vec2(-4.f, 2.f);
		potentials[5] += glm::vec2(-3.f,-3.f);
		potentials[6] += glm::vec2(-3.f, 3.f);
		potentials[7] += glm::vec2(-2.f,-4.f);
		potentials[8] += glm::vec2(-2.f, 4.f);
	}
	
	return potentials;
}

void Ball::generateListOfTiles(glm::vec2 A, glm::vec2 B) {

	vector<glm::vec2> potentialsA = computePotentials(A);

	vector<glm::vec2> potentialsCurr = potentialsA;
	vector<glm::vec2> potentialsNext = potentialsA;
	
	// quiero coger el potentials next, comprobar si hay colision, y si no hay y si todavia no ha llegado al punto B, vuelve a iterar

	for (glm::vec2& coord : potentialsNext) {
		coord = coord + glm::vec2(dir.x* 0.5, dir.y * 0.5);

	}

	vector<glm::vec2> potentialsB = computePotentials(B);
	
	
	
	/*
	glm::vec2( 0, 4)
		// Top Collision
		glm::vec2( 0, 4),
		glm::vec2( 1, 4),
		glm::vec2(-1, 4),
		glm::vec2( 2, 4),
		glm::vec2(-2, 4),
		glm::vec2( 3, 3),
		glm::vec2(-3, 3),
		glm::vec2( 4, 2),
		glm::vec2(-4, 2)

	glm::vec2( 1, 4)
		glm::vec2( 0, 4),
		glm::vec2( 1, 4),
		glm::vec2(-1, 4),
		glm::vec2( 2, 4),
		glm::vec2(-2, 4),
		glm::vec2( 3, 3),
		glm::vec2(-3, 3),
		glm::vec2( 4, 2),


	
	*/


}

void Ball::render()
{
	sprite->render();
}

void Ball::changeModifierX(float value)
{
	dirX = value;
}

void Ball::changeModifierY(float value)
{
	dirY = value;
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
