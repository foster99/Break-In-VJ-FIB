#include "Boss.h"

void Boss::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bossSize = glm::vec2(40, 40);
	speed = 2;
	spdModifierX = 1;
	spdModifierY = 1;
	movingRight = true;
	tileMapDispl = tileMapPos;

	std::default_random_engine generator;
	std::uniform_int_distribution<int> rand(1000, 3000);
	time2Switch= rand(generator);
	
	tex.loadFromFile("images/boss.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(bossSize, glm::vec2(1.f / 3.f,1.f/5.f), &tex, &shaderProgram);
	sprite->setNumberAnimations(15);

	sprite->addKeyframe(0,	glm::vec2(0.f,			0.f / 5.f));
	sprite->addKeyframe(1,	glm::vec2(1.f / 3.f,	0.f / 5.f));
	sprite->addKeyframe(2,	glm::vec2(2.f / 3.f,	0.f / 5.f));

	sprite->addKeyframe(3,	glm::vec2(0.f,			1.f / 5.f));
	sprite->addKeyframe(4,	glm::vec2(1.f / 3.f,	1.f / 5.f));
	sprite->addKeyframe(5,	glm::vec2(2.f / 3.f,	1.f / 5.f));
	
	sprite->addKeyframe(6,	glm::vec2(0.f,			2.f / 5.f));
	sprite->addKeyframe(7,	glm::vec2(1.f / 3.f,	2.f / 5.f));
	sprite->addKeyframe(8,	glm::vec2(2.f / 3.f,	2.f / 5.f));
	
	sprite->addKeyframe(9,	glm::vec2(0.f,			3.f / 5.f));
	sprite->addKeyframe(10,	glm::vec2(1.f / 3.f,	3.f / 5.f));
	sprite->addKeyframe(11,	glm::vec2(2.f / 3.f,	3.f / 5.f));
	
	sprite->addKeyframe(12,	glm::vec2(0.f,			4.f / 5.f));
	sprite->addKeyframe(13,	glm::vec2(1.f / 3.f,	4.f / 5.f));
	sprite->addKeyframe(14,	glm::vec2(2.f / 3.f,	4.f / 5.f));

	

	sprite->changeAnimation(13);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
}

bool Boss::update(int deltaTime)
{
	int mapX = map->getMapSizeX();
	int tileSize = map->getTileSize();
	elapsedTime += deltaTime;
	if (elapsedTime > time2Switch) {
		elapsedTime = 0;
		
		std::default_random_engine generator;
		std::uniform_int_distribution<int> rand(1000, 3000);
		time2Switch = rand(generator);

		movingRight = !movingRight;
	}

	if (movingRight) {
		if ((posBoss.x + bossSize.x) < ((mapX - 2) * tileSize))
			posBoss += glm::vec2(speed, 0);
		else
			movingRight = !movingRight;
	}
	else {
		if (posBoss.x > (tileSize*2))
			posBoss -= glm::vec2(speed, 0);
		else
			movingRight = !movingRight;
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
	
	return collisionWithPlayer();
}

void Boss::render(glm::mat4& displacement_mat)
{
	sprite->render(displacement_mat);
}

void Boss::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Boss::setPosition(const glm::vec2& pos)
{
	posBoss = pos;
}

void Boss::setPlayer(Player* p)
{
	player = p;
}

bool Boss::checkCollision()
{
	return false;
}

bool Boss::collisionWithPlayer() {

	float p0_x = player->getPosition().x;
	float p1_x = p0_x + player->getSize().x;
	float g0_x = posBoss.x;
	float g1_x = g0_x + bossSize.x;

	float p0_y = player->getPosition().y;
	float p1_y = p0_y + player->getSize().y;
	float g0_y = posBoss.y;
	float g1_y = g0_y + bossSize.y;

	bool X = (g0_x <= p0_x && p0_x <= g1_x) || (g0_x <= p1_x && p1_x <= g1_x);
	bool Y = (g0_y <= p0_y && p0_y <= g1_y) || (g0_y <= p1_y && p1_y <= g1_y);

	return (X && Y);
}


bool Boss::onSlide(const glm::ivec2& pos, int sizeX, float& modifierY, float& modifierX) {

	for (int x = 0; x < sizeX; ++x) {
		if ((pos.x + x) >= (posBoss.x) && (pos.x + x) <= (posBoss.x + bossSize.x)) {

			return true;
		}
	}
	return false;
}

bool Boss::onSide(const glm::ivec2& pos, int sizeY) {
	for (int y = 0; y <= sizeY; ++y) {
		if ((pos.y + y) >= posBoss.y && (pos.y + y) <= (posBoss.y + bossSize.y))
			return true;
	}
	return false;
}


bool Boss::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size, float* posJ, int speedBall)
{
	int rightLimit = pos.x + size.x;
	int speedBall2 = abs(speedBall);

	//if (movingRight) {
		for (int spb = 0; spb < speed; ++spb) {
			for (int s = 1; s <= speedBall2; ++s) {
				if ((rightLimit + s) == (posBoss.x + spb) && onSide(pos, size.y)) {
					*posJ += s - 1;
					return true;
				}
			}

		}
	//}

	//else {
		for (int spb = 0; spb >= (-1 * speed); --spb) {
			for (int s = 1; s <= speedBall2; ++s) {
				if ((rightLimit + s) == (posBoss.x + spb) && onSide(pos, size.y)) {
					*posJ += s - 1;
					return true;
				}
			}
		}
	//}


	return false;
}

bool Boss::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, float* posJ, int speedBall)
{
	int leftLimit = pos.x;
	int speedBall2 = abs(speedBall);

	//if (movingRight) {
		for (int spb = 0; spb < speed; ++spb) {
			for (int s = 1; s <= speedBall2; ++s) {
				if (leftLimit == (posBoss.x + bossSize.x + spb) && onSide(pos, size.y)) {
					*posJ -= s - 1;
					return true;
				}
			}
		}
	//}

	//else {
		for (int spb = 0; spb >= (-1 * speed); --spb) {
			for (int s = 1; s <= speedBall2; ++s) {
				if (leftLimit == (posBoss.x + bossSize.x + spb) && onSide(pos, size.y)) {
					*posJ -= s - 1;
					return true;
				}
			}
		}
	//}
		
	return false;
}


bool Boss::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, float* posI, int speed, float& modifierY, float& modifierX)
{
	int spd = int(speed + abs(modifierY));

	for (int s = 1; s <= spd; ++s) {
		if ((pos.y - s) == (posBoss.y + bossSize.y) && onSlide(pos, size.x, modifierY, modifierX)) {
			*posI -= s - 1;
			return true;
		}
	}
	return false;
}