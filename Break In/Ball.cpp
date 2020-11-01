#include "Ball.h"
#include "Game.h"


void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{	
	sizeBall = 9;
	speed = 2;
	spdModifierX = 1;
	spdModifierY = 1;

	tex.loadFromFile("images/ball.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(sizeBall, sizeBall), glm::vec2(1.f, 1.f), &tex, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
	
}

bool Ball::update(int deltaTime)
{
	bool collisionY = false;
	bool collisionX = false;
	bool collisionPlayer = false;

	if (collisionPlayer = (spdModifierY > 0 && player->collisionMoveDown(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.y, speed, spdModifierY, spdModifierX))) {
		spdModifierY *= -1;
	}
	else if (collisionPlayer = (spdModifierY > 0 && spdModifierX > 0 && player->collisionMoveRight(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.y, int(speed * spdModifierY)))) {
		spdModifierX *= -1;
	}
	else if (collisionPlayer = (spdModifierY > 0 && spdModifierX < 0 && player->collisionMoveLeft(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.y, int(speed * spdModifierY)))) {
		spdModifierX *= -1;
	}
	
	if (!collisionPlayer) {
		if (collisionY = (spdModifierY > 0 && map->collisionMoveDown( posBall, glm::ivec2(sizeBall, sizeBall), &posBall.y, speed, spdModifierY)))
			spdModifierY *= -1;
		else if (collisionY = (spdModifierY < 0 && map->collisionMoveUp(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.y, speed, spdModifierY)))
			spdModifierY *= -1;

		if (collisionX = (spdModifierX > 0 && map->collisionMoveRight(posBall, glm::ivec2(sizeBall, sizeBall), &posBall.x, speed, spdModifierX)))
			spdModifierX *= -1;
		else if (collisionX = (spdModifierX < 0 && map->collisionMoveLeft( posBall, glm::ivec2(sizeBall, sizeBall), &posBall.x, speed, spdModifierX)))
			spdModifierX *= -1;
	}
	
	if (collisionX || collisionY || collisionPlayer) Game::instance().playBallSound();


	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
	
	// chapuza para contar colisiones, habria que cambiarlo por el retorno de un entero o un identificador
	// de la tile con la que ha chocado etc.
	return (collisionPlayer);
}

void Ball::render(glm::mat4& displacement_mat)
{
	sprite->render(displacement_mat);
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

glm::vec2 Ball::getPosition()
{
	return posBall;
}
