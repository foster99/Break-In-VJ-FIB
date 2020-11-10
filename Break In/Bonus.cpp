#include "Bonus.h"

void Bonus::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	// INITIALIZATIONS
	sizeBonus = 13;
	posBonus = glm::vec2(0.f);
	speed = 0.3;
	spdModifierX = 1;
	spdModifierY = 1;
	activeBonus = blaster; 
	bonusTime = 0.f;
	tileMapDispl = tileMapPos;

	// SPRITE AND TEXTURE SET-UP
	tex.loadFromFile("images/bonus.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(sizeBonus, sizeBonus), glm::vec2(1.f / 6.f, 1.f / 2.f), &tex, &shaderProgram);
	sprite->setNumberAnimations(6);

	sprite->addKeyframe(multipleBall,	glm::vec2(0.f / 6.f, 0.f));
	sprite->addKeyframe(blaster,		glm::vec2(1.f / 6.f, 0.f));
	sprite->addKeyframe(doubleSlide,	glm::vec2(2.f / 6.f, 0.f));
	sprite->addKeyframe(magnet,			glm::vec2(3.f / 6.f, 0.f));
	sprite->addKeyframe(twix,			glm::vec2(4.f / 6.f, 0.f));
	sprite->addKeyframe(none,			glm::vec2(4.f / 6.f, 0.f));

	//sprite->addKeyframe(6 + multipleBall,	glm::vec2(0.f / 6.f, 1.f / 2.f));
	//sprite->addKeyframe(6 + blaster,		glm::vec2(1.f / 6.f, 1.f / 2.f));
	//sprite->addKeyframe(6 + doubleSlide,	glm::vec2(2.f / 6.f, 1.f / 2.f));
	//sprite->addKeyframe(6 + magnet,			glm::vec2(3.f / 6.f, 1.f / 2.f));
	//sprite->addKeyframe(6 + twix,			glm::vec2(4.f / 6.f, 1.f / 2.f));

	sprite->changeAnimation(doubleSlide);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBonus.x), float(tileMapDispl.y + posBonus.y)));
}

bool Bonus::update(int deltaTime)
{
	// CHECK IF IT'S TIME FOR THE BONUS
	bonusTime += deltaTime;
	if (bonusTime < bonusDelayTime) {
		sprite->changeAnimation(activeBonus);
		timeToRender = false;
		return false;
	}
	if (bonusTime > bonusDelayTime + bonusAliveTime)
	{
		restartTime();
		timeToRender = false;
		return false;
	}
	timeToRender = true;
	
	// MODIFICATE BONUS TYPE
	bonusTypeTime += deltaTime;
	if (bonusTypeTime > 3000) {
		bonusTypeTime = 0;
		activeBonus = (++activeBonus) % 5;
		sprite->changeAnimation(activeBonus);
	}

	// CHECK COLLISION WITH WALLS
	glm::ivec2 maxPos, minPos;
	
	// X LIMITS
	minPos.x = 0;
	maxPos.x = map->getMapSizeX() * map->getTileSize();
	
	// Y LIMITS
	switch (room)
	{
	case 1:
		minPos.y = 2 * 24 * map->getTileSize();
		maxPos.y = 3 * 24 * map->getTileSize();
		break;
	case 2:
		minPos.y = 1 * 24 * map->getTileSize();
		maxPos.y = 2 * 24 * map->getTileSize();
		break;
	case 3:
		minPos.y = 0;
		maxPos.y = 1 * 24 * map->getTileSize();
		break;
	default: break;
	}

	// DIRECTION CORRECTION
	if (int(posBonus.x) <= minPos.x || maxPos.x <= int(posBonus.x) + sizeBonus) spdModifierX *= -1;
	if (int(posBonus.y) <= minPos.y || maxPos.y <= int(posBonus.y) + sizeBonus) spdModifierY *= -1;

	// NEXT POSITION COMPUTATION
	posBonus.x += speed * spdModifierX;
	posBonus.y += speed * spdModifierY;

	sprite->setPosition(glm::vec2(	float(tileMapDispl.x + posBonus.x),
									float(tileMapDispl.y + posBonus.y)));
	
	// CHECK COLLISION WITH PLAYER IN PREVIOUS FRAME
	if (checkCollision()) {
		sprite->changeAnimation(6 + activeBonus);
		restartTime();
		return true;
	}

	return false;
}

void Bonus::render(glm::mat4& displacement_mat)
{
	if (timeToRender)	sprite->render(displacement_mat);
}

void Bonus::changeModifierX(float value)
{
	spdModifierX = value;
}

void Bonus::changeModifierY(float value)
{
	spdModifierY = value;
}

void Bonus::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Bonus::setPosition(const glm::vec2& pos)
{
	posBonus = pos;
}

void Bonus::setPlayer(Player* p)
{
	player = p;
}

void Bonus::setRoom(int r)
{
	room = r;
}

void Bonus::restartTime()
{
	switch (room) {
	case 1:	posBonus = glm::vec2(2.f * map->getTileSize(), (3 * 24 - 5) * map->getTileSize()); break;
	case 2:	posBonus = glm::vec2(2.f * map->getTileSize(), (2 * 24 - 5) * map->getTileSize()); break;
	case 3:	posBonus = glm::vec2(2.f * map->getTileSize(), (1 * 24 - 5) * map->getTileSize()); break;
	default: break;
	}
	bonusTime = 0.f;
}

bool Bonus::isTimeToRender()
{
	return timeToRender;
}

int Bonus::getActiveBonus()
{
	return activeBonus;
}

bool Bonus::checkCollision()
{
	float p0_x = player->getPosition().x;
	float p1_x = p0_x + player->getSize().x;
	float g0_x = posBonus.x;
	float g1_x = g0_x + sizeBonus;

	float p0_y = player->getPosition().y;
	float p1_y = p0_y + player->getSize().y;
	float g0_y = posBonus.y;
	float g1_y = g0_y + sizeBonus;

	bool X = (p0_x <= g0_x && g0_x <= p1_x) || (p0_x <= g1_x && g1_x <= p1_x);
	bool Y = (p0_y <= g0_y && g0_y <= p1_y) || (p0_y <= g1_y && g1_y <= p1_y);

	return X && Y;
}

glm::vec2 Bonus::getPosition()
{
	return posBonus;
}

glm::ivec2 Bonus::getPositionInTiles()
{
	return glm::ivec2(posBonus.x / map->getTileSize(), posBonus.y / map->getTileSize());
}