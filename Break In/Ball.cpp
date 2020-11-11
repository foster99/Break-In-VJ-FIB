#include "Ball.h"
#include "Game.h"



void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, float spdX, float spdY, bool freeze)
{
	sizeBall = 9;
	speed = 2;
	spdModifierX = spdX;
	spdModifierY = spdY;
	magnet = freeze;
	bossFight = false;

	tex.loadFromFile("images/balls.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(sizeBall, sizeBall), glm::vec2(1.f / 5.f, 1.f), &tex, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(1, glm::vec2(1.f / 5.f, 0.f));
	sprite->addKeyframe(2, glm::vec2(2.f / 5.f, 0.f));
	sprite->addKeyframe(3, glm::vec2(3.f / 5.f, 0.f));
	sprite->addKeyframe(4, glm::vec2(4.f / 5.f, 0.f));

	sprite->changeAnimation(0);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

bool Ball::update(int deltaTime)
{
	bool collisionY = false;
	bool collisionX = false;
	bool collisionPlayer = false;
	bool collisionBoss = false;
	sprite->changeAnimation(0);

	if (bossFight) {
		if (collisionBoss = (spdModifierY < 0 && boss->collisionMoveUp(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.y, (int)speed, spdModifierY, spdModifierX))) {
			sprite->changeAnimation(3);
			spdModifierY *= -1;
		}
		if (collisionBoss = (spdModifierX > 0 && boss->collisionMoveRight(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.y, int(speed * spdModifierY)))) {
			spdModifierX *= -1;
			if (spdModifierY < 0) spdModifierY *= -1;
			sprite->changeAnimation(1);
		}
		else if (collisionBoss = (spdModifierX < 0 && boss->collisionMoveLeft(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.y, int(speed * spdModifierY)))) {
			spdModifierX *= -1;
			if (spdModifierY < 0) spdModifierY *= -1;
			sprite->changeAnimation(2);
		}
	}

	if (!collisionBoss) {
		if (collisionPlayer = (spdModifierY > 0 && player->collisionMoveDown(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.y, (int)speed, spdModifierY, spdModifierX))) {
			spdModifierY *= -1;
			sprite->changeAnimation(4);
		}
		else if (collisionPlayer = (spdModifierY > 0 && spdModifierX > 0 && player->collisionMoveRight(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.y, int(speed * spdModifierY)))) {
			spdModifierX *= -1;
			sprite->changeAnimation(1);
		}
		else if (collisionPlayer = (spdModifierY > 0 && spdModifierX < 0 && player->collisionMoveLeft(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.y, int(speed * spdModifierY)))) {
			spdModifierX *= -1;
			sprite->changeAnimation(2);
		}
	}

	if (!collisionPlayer && !collisionBoss)
	{
		if (collisionY = (spdModifierY > 0 && map->collisionMoveDown(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.y, (int)speed, spdModifierY, lastCollision))) {
			sprite->changeAnimation(4);
			spdModifierY *= -1;
		}
		else if (collisionY = (spdModifierY < 0 && map->collisionMoveUp(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.y, (int)speed, spdModifierY, lastCollision))){
			sprite->changeAnimation(3);
			spdModifierY *= -1;
		}

		if (collisionX = (spdModifierX > 0 && map->collisionMoveRight(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.x, (int)speed, spdModifierX, lastCollision))){
			sprite->changeAnimation(1);
			spdModifierX *= -1;
		}
		else if (collisionX = (spdModifierX < 0 && map->collisionMoveLeft( posBall, glm::ivec2(sizeBall, sizeBall), &posBall.x, (int) speed, spdModifierX, lastCollision))){
			sprite->changeAnimation(2);
			spdModifierX *= -1;
		}
	}
	
	if (collisionPlayer)	lastCollision = glm::ivec2(-1);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));

	return (collisionPlayer || collisionX || collisionY);
}

void Ball::render(glm::mat4& displacement_mat)
{
	sprite->render(displacement_mat);
}

void Ball::moveWithPlayer(float spdX)
{

	posBall += glm::vec2(spdX,0);
	if( (posBall.x / map->getTileSize()) > (map->getMapSizeX()-2) || (posBall.x / map->getTileSize()) < 1)
		posBall -= glm::vec2(spdX, 0);
	sprite->setPosition(posBall);
}

void Ball::toogleMagnet()
{
	magnet = !magnet;
}

void Ball::toogleBossFight()
{
	bossFight = !bossFight;
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

void Ball::setPlayer(Player* pla)
{
	player = pla;
}


void Ball::setPosition(const glm::vec2& pos)
{
	posBall = pos;
	sprite->setPosition(posBall);
}

void Ball::setBoss(Boss* b)
{
	boss = b;
}

bool Ball::getMagnet() {
	return magnet;
}

glm::vec2 Ball::getPosition()
{
	return posBall;
}

glm::ivec2 Ball::getPositionInTiles()
{
	return glm::ivec2(posBall.x/map->getTileSize(), posBall.y / map->getTileSize());
}

glm::ivec2 Ball::getBasePositionInTiles()
{
	return glm::ivec2((posBall.x + sizeBall) / map->getTileSize(), (posBall.y + sizeBall) / map->getTileSize());
}

int Ball::getBallSize()
{
	return sizeBall;
}

glm::ivec2 Ball::getLastCollision()
{
	return lastCollision;
}
