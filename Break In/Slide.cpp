#include "Player.h"
#include "Slide.h"

void Slide::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	singleSize	= glm::ivec2(19, 6);
	quadSize	= glm::ivec2(40, 6);
	logicSize = quadSize;

	tex.loadFromFile("images/bars.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(quadSize, glm::vec2(1.f, 0.2f), &tex, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(1, glm::vec2(0.f, 0.2f));
	sprite->addKeyframe(2, glm::vec2(0.f, 0.4f));
	sprite->addKeyframe(3, glm::vec2(0.f, 0.6f));
	sprite->addKeyframe(4, glm::vec2(0.f, 0.8f));

	sprite->changeAnimation(4);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSlide.x), float(tileMapDispl.y + posSlide.y)));


}

void Slide::update(int deltaTime)
{
}


void Slide::render(glm::mat4& displacement_mat)
{
	sprite->render(displacement_mat);
}

glm::ivec2 Slide::getLogicSize()
{
	return logicSize;
}

void Slide::setPosition(const glm::vec2& pos)
{
	posSlide = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSlide.x), float(tileMapDispl.y + posSlide.y)));
}

void Slide::setOffSets(int offX, int offY)
{
	offSet = glm::ivec2(offX, offY);
}


void Slide::toogleChangeBar() 
{
	if (logicSize == quadSize) {
		logicSize = singleSize;
		sprite->changeAnimation(2);
	}		
	else {
		logicSize = quadSize;
		sprite->changeAnimation(4);
	}
		
}

void changeMods(int x, float& modifierY, float& modifierX, bool big) {
	if (! big) {
		if (x < 5) {
			modifierY = 1;
			modifierX = -1.2;
		}
		else if (x < 10) {
			modifierY = 1;
			modifierX = -0.5;
		}
		else if (x < 15) {
			modifierY = 1;
			modifierX = 0.5;
		}
		else {
			modifierY = 1;
			modifierX = 1.2;
		}
	}
	else {
		if (x < 5) {
			modifierY = 1;
			modifierX = -1.2;
		}
		else if (x < 20) {
			modifierY = 1;
			modifierX = -0.5;
		}
		else if (x < 35) {
			modifierY = 1;
			modifierX = 0.5;
		}
		else {
			modifierY = 1;
			modifierX = 1.2;
		}
	}
}

bool Slide::onSlide(const glm::ivec2& pos, int sizeX, float& modifierY, float& modifierX) {
	int diff = 0;
	if (logicSize != quadSize)
		diff = offSet.x;

	for (int x = 0; x < sizeX; ++x) {
		if ((pos.x + x) >= (posSlide.x + diff) && (pos.x + x) <= (posSlide.x + logicSize.x + diff)) {
			
			if (diff == 0)
				changeMods(abs(posSlide.x - (pos.x + sizeX/2)),modifierY, modifierX, true);
			else
				changeMods(abs(posSlide.x - (pos.x + sizeX/2) + diff),modifierY, modifierX, false);

			return true;
		}
	}
	return false;
}


bool Slide::onSide(const glm::ivec2& pos, int sizeY) {
	for (int y = 0; y <= sizeY; ++y) {
		if ((pos.y + y) >= posSlide.y && (pos.y + y) <= (posSlide.y + logicSize.y))
			return true;
	}
	return false;
}

bool Slide::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, float* posI, int speed, float& modifierY, float& modifierX)
{
	int spd = int(speed + abs(modifierY));
	
	for (int s = 1; s <= spd; ++s) {
		if ((pos.y + s + size.y) == posSlide.y && onSlide(pos, size.x, modifierY, modifierX)) {
			*posI += s - 1;
			return true;
		}
	}
	return false;
}

bool Slide::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size, float* posJ, int speed)
{
	int diff = 0;
	if (logicSize != quadSize)
		diff = offSet.x;

	int rightLimit = pos.x + size.x;

	for (int s = 1; s <= speed; ++s) {
		if ((rightLimit + s) == (posSlide.x + diff) && onSide(pos, size.y)) {
			*posJ += s - 1;
			return true;
		}
	}
	return false;
}

bool Slide::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, float* posJ, int speed)
{
	int diff = 0;
	if (logicSize != quadSize)
		diff = offSet.x;

	int leftLimit = pos.x;

	for (int s = 1; s <= speed; ++s) {
		if (leftLimit == (posSlide.x + logicSize.x + diff) && onSide(pos, size.y)) {
			*posJ -= s - 1;
			return true;
		}
	}
	return false;
}
