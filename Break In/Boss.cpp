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
	
	tex.loadFromFile("images/bee.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(bossSize, glm::vec2(1.f,1.f), &tex, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);
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
		if ((posBoss.x + bossSize.x) < ((mapX - 1) * tileSize))
			posBoss += glm::vec2(speed, 0);
		else
			movingRight = !movingRight;
	}
	else {
		if (posBoss.x > tileSize)
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

void Boss::setBall(Ball* b)
{
	ball = b;
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

	bool X = (p0_x <= g0_x && g0_x <= p1_x) || (p0_x <= g1_x && g1_x <= p1_x);
	bool Y = (p0_y <= g0_y && g0_y <= p1_y) || (p0_y <= g1_y && g1_y <= p1_y);

	return X && Y;
}

bool Boss::collisionWithBall()
{
	float p0_x = ball->getPosition().x;
	float p1_x = p0_x + ball->getBallSize();
	float g0_x = posBoss.x;
	float g1_x = g0_x + bossSize.x;

	float p0_y = ball->getPosition().y;
	float p1_y = p0_y + ball->getBallSize();
	float g0_y = posBoss.y;
	float g1_y = g0_y + bossSize.y;

	bool X = (p0_x <= g0_x && g0_x <= p1_x) || (p0_x <= g1_x && g1_x <= p1_x);
	bool Y = (p0_y <= g0_y && g0_y <= p1_y) || (p0_y <= g1_y && g1_y <= p1_y);

	return X && Y;
}
