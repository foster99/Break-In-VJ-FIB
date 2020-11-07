#include "Guardian.h"

void Guardian::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	// INITIALIZATIONS
	sizeGuardian = glm::ivec2(13, 8);
	posGuardian = glm::vec2(0.f);
	guardianRoom = 0;
	targetPos = glm::vec2(0.f);
	speed = 0.4;
	speedModifierX = 1;
	speedModifierY = 1;
	guardianMode = sleep;
	guardianTime = 0.f;
	tileMapDispl = tileMapPos;
	animation = 3;
	count = 0;
	dir = 1;

	// SPRITE AND TEXTURE SET-UP
	tex.loadFromFile("images/guardian.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeGuardian, glm::vec2(1.f / 7.f, 1.f), &tex, &shaderProgram);
	sprite->setNumberAnimations(7);
	sprite->addKeyframe(0, glm::vec2(      0.f, 0.f));
	sprite->addKeyframe(1, glm::vec2(1.f / 7.f, 0.f));
	sprite->addKeyframe(2, glm::vec2(2.f / 7.f, 0.f));
	sprite->addKeyframe(3, glm::vec2(3.f / 7.f, 0.f));
	sprite->addKeyframe(4, glm::vec2(4.f / 7.f, 0.f));
	sprite->addKeyframe(5, glm::vec2(5.f / 7.f, 0.f));
	sprite->addKeyframe(6, glm::vec2(6.f / 7.f, 0.f));
	sprite->changeAnimation(animation);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGuardian.x), float(tileMapDispl.y + posGuardian.y)));
}

bool Guardian::update(int deltaTime) {

	switch (guardianMode) {
	case sleep:	return false;
	case tracking:
		guardianTime += deltaTime;
		if (guardianTime < trackingTime)
			nextAnimation();
		else {
			trackPlayerPosition();
			guardianMode = moving;
		}
		break;
	case moving:
		if (arrivedTargetPos()) {
			guardianTime = 0.f;
			guardianMode = tracking;
		}
		else {
			posGuardian.x += speed * speedModifierX;
			posGuardian.y += speed * speedModifierY;
		}
		break;
	}


	sprite->setPosition(glm::vec2(	float(tileMapDispl.x + posGuardian.x),
									float(tileMapDispl.y + posGuardian.y)));

	return checkCollision();
}

void Guardian::render(glm::mat4& displacement_mat)
{
	if (guardianMode != sleep) {
		sprite->changeAnimation(animation);
		sprite->render(displacement_mat);
	}
}

void Guardian::alarmOn()
{
	if (!isAwake()) guardianMode = tracking;
}

void Guardian::nextAnimation()
{
	if (++count == 2) {
			 if (animation == 0) dir =  1;
		else if (animation == 6) dir = -1;
		animation += dir;
		count = 0;
	}
}

bool Guardian::arrivedTargetPos()
{
	return glm::length(targetPos - posGuardian) < 8;
}

void Guardian::trackPlayerPosition()
{
	targetPos = player->getPosition();

	glm::vec2 V = glm::normalize(targetPos - posGuardian);
	speedModifierX = V.x;
	speedModifierY = V.y;
}

void Guardian::changeModifierX(float value)
{
	speedModifierX = value;
}

void Guardian::changeModifierY(float value)
{
	speedModifierY = value;
}

int Guardian::getRoom()
{
	return guardianRoom;
}

void Guardian::setRoom(int r)
{
	posGuardian.y = map->getTileSize() * (72 - 24 * (r - 1) - 2);
	guardianRoom = r;
}

void Guardian::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Guardian::setPosition(const glm::vec2& pos)
{
	posGuardian = pos;
}

void Guardian::setPlayer(Player* p)
{
	player = p;
}

void Guardian::restartTime()
{
	guardianTime = 0.f;
}

bool Guardian::isAwake()
{
	return guardianMode != 0;
}

bool Guardian::checkCollision()
{
	float p0_x = player->getPosition().x;
	float p1_x = p0_x + player->getSize().x;
	float g0_x = posGuardian.x;
	float g1_x = g0_x + sizeGuardian.x;

	float p0_y = player->getPosition().y;
	float p1_y = p0_y + player->getSize().y;
	float g0_y = posGuardian.y;
	float g1_y = g0_y + sizeGuardian.y;

	bool X = (p0_x <= g0_x && g0_x <= p1_x) || (p0_x <= g1_x && g1_x <= p1_x);
	bool Y = (p0_y <= g0_y && g0_y <= p1_y) || (p0_y <= g1_y && g1_y <= p1_y);

	return X && Y;
}

glm::vec2 Guardian::getPosition()
{
	return posGuardian;
}

glm::ivec2 Guardian::getPositionInTiles()
{
	return getPosition() * glm::vec2(map->getTileSize());
}
