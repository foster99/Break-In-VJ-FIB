#include "Boss.h"

void Boss::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bossSize = glm::vec2(60, 60);
	speed = 2;
	spdModifierX = 1;
	spdModifierY = 1;
	movingRight = true;
	tileMapDispl = tileMapPos;

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
	return false;
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
