#include "Boss.h"
#include "Game.h"
#include <random>

void Boss::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bossSize = glm::vec2(40, 40);
	speed = 2;
	spdModifierX = 1;
	spdModifierY = 1;
	fase = 1;
	alive = true;
	movingRight = true;
	tileMapDispl = tileMapPos;
	brickTime = 0.f;
	statusTime = 0.f;
	shield1 = false;
	shield2 = false;
	status = NORMAL;
	hunterMode = SLEEP;
	actTracking = 0;
	healthPoints = 100;
	shieldDurability = 100;

	fase1_status = waiting;

	std::default_random_engine generator;
	std::uniform_int_distribution<int> rand(1000, 3000);
	time2Switch= rand(generator);
	
	prog = shaderProgram;


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

	

	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
}

bool Boss::update(int deltaTime)
{
	// FEATURES

	hitted = false;
	int mapX = map->getMapSizeX();
	int tileSize = map->getTileSize();
	elapsedTime += deltaTime;
	
	if (fase < 3) {
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
			if (posBoss.x > (tileSize * 2))
				posBoss -= glm::vec2(speed, 0);
			else
				movingRight = !movingRight;
		}
	}
	
	// RANDOM BRICKS
	brickTime += deltaTime;
	if (brickTime > 1000.f)
	{
		std::random_device rd;
		int i = 48 + 7 + (rd() % 6);
		int j = 1 + (rd() % 20);
		map->insertBrick(i, j);

		brickTime = 0.f;
	}


	if (fase == 1) {	

		if (fase1_status == waiting)	fase1_status = part1;

		if (fase1_status == part1 && healthPoints > 0) {		// modo normal
			status = Boss::NORMAL;
			statusTime = 0.f;
			if (healthPoints <= 50) {
				fase1_status = part2;
				statusTime = 0.f;
			}
		}
		else if (fase1_status == part2 && healthPoints > 0)		// modo stunned
		{
			status = Boss::FIRE;		// te stunea
			statusTime += deltaTime;
			if (statusTime > 4000.f) {	// si llevas 4sec stuned
				status = Boss::SHIELD;	// te pones escudo
				fase1_status = part3;
			}
		}
		else if (fase1_status == part3 && healthPoints > 0)		// modo escudo
		{
			shield1 = true;
			shield2 = true;
			map->insertShield1();
			map->insertShield2();
			fase1_status = part4;
		}
		else if (fase1_status == part4 && (!shield1 ^ !shield2) && healthPoints > 0)
		{
			status = Boss::BROKENSHIELD;
			fase1_status = part5;
		}
		else if (fase1_status == part5 && !shield1 && !shield2 && healthPoints > 0)
		{
			status = Boss::NORMAL;
			fase1_status = part6;
		}
		else if (fase1_status == part6 && healthPoints <= 0)
		{
			fase1_status = done;
		}
		else if (fase1_status == done){
			fase++;
			fase2_status = waiting;
		}
	}
	else if (bank > 1 && fase == 2) {	// FASE 2
		if (fase2_status == waiting) {
			healthPoints = 100;
			status = FIRE;
			numberGuardians = 0;
			elapsedTime = 0.f;
			fase2_status = part1;
		}
		else if (fase2_status == part1 && elapsedTime >= createGuardian) {
			elapsedTime = 0.f;
			if (numberGuardians == 0)
				createNewGuardian(2, 52);
			else if (numberGuardians == 1)
				createNewGuardian(18, 52);
			else
				fase2_status = part2;
		}
		else if (fase2_status == part2 && healthPoints > 0) {
			for (Guardian* guard : guardians) {
				guard->update(deltaTime);
			}
		}
		else if (fase2_status == part2 && healthPoints <= 0) {
			fase2_status = done;
		}
		else if (fase1_status == done) {
			fase++;
			fase3_status = waiting;
		}

		
	}
	else if (bank > 2 && fase == 3) {	// FASE 3
		map->closeDeathDoor();
		switch (hunterMode) {
			case SLEEP: break;
				break;
			case TRACKING:
				actTracking += deltaTime;
				if (actTracking < trackingTime) {
					nextAnimation();
				}
				else {
					trackPlayerPosition();
					hunterMode = MOVING;
				}
				break;
			case MOVING:
				if (arrivedTargetPos()) {
					actTracking = 0;
					hunterMode = TRACKING;
				}
				else {
					posBoss.x += speed * spdModifierX;
					posBoss.y += speed * spdModifierY;
				}
				break;
		}
	}
	else {								// END BOSS FIGHT
		alive = false;
	}

	switch (status)
	{
	case NORMAL:		sprite->changeAnimation(1);		break;
	case STUNED:		sprite->changeAnimation(4);		break;
	case FIRE:			sprite->changeAnimation(7);		break;
	case SHIELD:		sprite->changeAnimation(10);	break;
	case BROKENSHIELD:	sprite->changeAnimation(13);	break;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));

	return collisionWithPlayer();
}

void Boss::render(glm::mat4& displacement_mat)
{
	sprite->render(displacement_mat);
	if (fase == 2) {
		for (Guardian* guard : guardians) {
			guard->render(displacement_mat);
		}
	}
		
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

void Boss::setShield1(bool s)
{
	shield1 = s;
	if (!s) {
		Game::instance().playBombSound();
		map->deleteShield1();
	}
}

void Boss::setShield2(bool s)
{
	shield2 = s;
	if (!s) {
		Game::instance().playBombSound();
		map->deleteShield2();
	}
}

bool Boss::isAlive()
{
	return alive;
}

void Boss::setFase(int f)
{
	fase = f;
}

void Boss::setStatus(int s)
{
	status = s;
}

void Boss::setHunterMode(int hm)
{
	hunterMode = hm;
}

void Boss::setBank(int b)
{
	bank = b;
}

void Boss::nextAnimation()
{
	
}

void Boss::trackPlayerPosition()
{
	targetPos = player->getPosition();

	glm::vec2 V = glm::normalize(targetPos - posBoss);
	spdModifierX = V.x;
	spdModifierY = V.y;
}

void Boss::createNewGuardian(int i, int j)
{
	guardian = new Guardian();
	guardian->init(glm::ivec2(0,0), prog);
	guardian->setTileMap(map);
	guardian->setPosition(glm::vec2(i * map->getTileSize(), j * map->getTileSize()));
	guardian->setPlayer(player);
	guardian->setRoom(map->getGuardianRoom());
	guardian->toogleBossFight();
	guardians.push_back(guardian);
	++numberGuardians;
}

bool Boss::arrivedTargetPos()
{
	return glm::length(targetPos - posBoss) < 8;
}

void Boss::takeDamage(int hp, int source)
{
	if (status == Boss::SHIELD || status == Boss::BROKENSHIELD || status == Boss::FIRE)
	{
		Game::instance().playShieldSound();
		return;
	}
	healthPoints -= hp;
	if (healthPoints < 0)
		healthPoints = 0;
	hitted = true;
	Game::instance().playBossHitSound();
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