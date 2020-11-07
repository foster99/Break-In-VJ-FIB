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
	sprite = Sprite::createSprite(glm::ivec2(sizeBonus, sizeBonus), glm::vec2(1.f / 5.f, 1.f), &tex, &shaderProgram);
	sprite->setNumberAnimations(5);
	sprite->addKeyframe(multipleBall,	glm::vec2(0.f, 0.f));
	sprite->addKeyframe(blaster,		glm::vec2(1.f / 5.f, 0.f));
	sprite->addKeyframe(doubleSlide,	glm::vec2(2.f / 5.f, 0.f));
	sprite->addKeyframe(magnet,			glm::vec2(3.f / 5.f, 0.f));
	sprite->addKeyframe(twix,			glm::vec2(4.f / 5.f, 0.f));
	sprite->changeAnimation(doubleSlide);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBonus.x), float(tileMapDispl.y + posBonus.y)));
}

bool Bonus::update(int deltaTime)
{
	// CHECK IF IT'S TIME FOR THE BONUS
	bonusTime += deltaTime;
	if (bonusTime < 5000) {
		timeToRender = false;
		return false;
	}
	if (bonusTime > 5000 + bonusAliveTime)
	{
		bonusTime = 0;
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

	if (posBonus.x + sizeBonus > map->getMapSizeX() * map->getTileSize() || posBonus.x <= 0) spdModifierX *= -1;
	if (posBonus.y + sizeBonus > map->getMapSizeY() * 3 * map->getTileSize() || posBonus.y <= 0) spdModifierY *= -1;

	posBonus.x += speed * spdModifierX;
	posBonus.y += speed * spdModifierY;

	sprite->setPosition(glm::vec2(	float(tileMapDispl.x + posBonus.x),
									float(tileMapDispl.y + posBonus.y)));
	
	bool collision_player = false;

	// ######################
	// check collision player
	// ######################

	return collision_player;
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

void Bonus::restartTime()
{
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

glm::vec2 Bonus::getPosition()
{
	return posBonus;
}

glm::ivec2 Bonus::getPositionInTiles()
{
	return glm::ivec2(posBonus.x / map->getTileSize(), posBonus.y / map->getTileSize());
}