#include "Bullet.h"

void Bullet::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	sizeBullet = glm::ivec2(4,6);
	speed = 2;
	destroy = false;

	tex.loadFromFile("images/bee.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeBullet, glm::vec2(0.5f, 0.5f), &tex, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(1, glm::vec2(0.5f, 0.5f));

	sprite->changeAnimation(0);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBullet.x), float(tileMapDispl.y + posBullet.y)));
}

bool Bullet::update(int deltaTime)
{
	bool collisionY = false;
	float mdf = -1;
	collisionY = map->collisionMoveUpB(posBullet,sizeBullet, &posBullet.y, (int)speed, mdf, lastCollision);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBullet.x), float(tileMapDispl.y + posBullet.y)));

	return collisionY;
}

void Bullet::render(glm::mat4& displacement_mat)
{
	sprite->render(displacement_mat);
}

bool Bullet::getDestroy()
{
	return destroy;
}

void Bullet::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Bullet::setPosition(const glm::vec2& pos)
{
	posBullet = pos;
}

void Bullet::setDestroy()
{
	destroy = true;
	sprite->changeAnimation(1);
}

glm::ivec2 Bullet::getLastCollision()
{
	return lastCollision;
}
