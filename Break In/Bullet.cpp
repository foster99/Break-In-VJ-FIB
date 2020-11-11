#include "Bullet.h"

void Bullet::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	sizeBullet = glm::ivec2(4,6);
	speed = 2;
	destroy = false;
	bossFight = false;

	tex.loadFromFile("images/bullet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeBullet, glm::vec2(1/5.f, 1.0f), &tex, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->addKeyframe(0, glm::vec2(0.f, 1.0f));
	sprite->addKeyframe(1, glm::vec2(1/5.f, 1.f));
	sprite->addKeyframe(1, glm::vec2(2/5.f, 1.f));
	sprite->addKeyframe(1, glm::vec2(3/5.f, 1.f));
	sprite->addKeyframe(1, glm::vec2(4/5.f, 1.f));

	sprite->changeAnimation(0);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBullet.x), float(tileMapDispl.y + posBullet.y)));
}

bool Bullet::update(int deltaTime, int& collided)
{
	bool collisionY = false;
	bool collisionBoss = false;
	float mdfY = -1;
	float mdfX = 0;
	if (bossFight) {
		if (collisionBoss = boss->collisionMoveUp(posBullet, sizeBullet, &posBullet.y, (int)speed, mdfY, mdfX))
			collided = 2;
	}
		
	if (collisionY = map->collisionMoveUpB(posBullet, sizeBullet, &posBullet.y, (int)speed, mdfY, lastCollision))
		collided = 0;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBullet.x), float(tileMapDispl.y + posBullet.y)));

	return (collisionY || collisionBoss);
}

void Bullet::render(glm::mat4& displacement_mat)
{
	sprite->render(displacement_mat);
}

void Bullet::toogleBossFight()
{
	bossFight = !bossFight;
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

void Bullet::setBoss(Boss* b)
{
	boss = b;
}

glm::ivec2 Bullet::getLastCollision()
{
	return lastCollision;
}
