#include "Player.h"
#include "Game.h"
#include "Bonus.h"

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{	
	sizePlayer = glm::ivec2(19,16);
	displ_posPlayer = mod(posPlayer, glm::vec2(24.f));
	speedX = 3;
	speedY = 2;
	animation = 2;
	bonus = -1;
	spritesheet.loadFromFile("images/eyes.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizePlayer, glm::vec2(1.f/3.f, 1.f/4.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(12);
	
	sprite->addKeyframe(0,	glm::vec2(0.f,		0.f));
	sprite->addKeyframe(1,	glm::vec2(1.f/3.f,	0.f));
	sprite->addKeyframe(2,	glm::vec2(2.f/3.f,	0.f));
	sprite->addKeyframe(3,	glm::vec2(0.f,		1.f/4.f));
	sprite->addKeyframe(4,	glm::vec2(1.f/3.f,	1.f/4.f));
	sprite->addKeyframe(5,	glm::vec2(2.f/3.f,	1.f/4.f));
	sprite->addKeyframe(6,	glm::vec2(0.f,		1.f/2.f));
	sprite->addKeyframe(7,	glm::vec2(1.f/3.f,	1.f/2.f));
	sprite->addKeyframe(8,	glm::vec2(2.f/3.f,	1.f/2.f));
	sprite->addKeyframe(9,	glm::vec2(0.f,		3.f/4.f));
	sprite->addKeyframe(10, glm::vec2(1.f/3.f,	3.f/4.f));
	sprite->addKeyframe(11, glm::vec2(2.f/3.f,	3.f/4.f));



	sprite->changeAnimation(animation);

	slideOffset.x = 10;
	slideOffset.y = 6;

	slide = new Slide();
	slide->init(tileMapPos, shaderProgram);
	slide->setPosition(posPlayer - (glm::vec2) slideOffset);
	slide->setOffSets(slideOffset.x, slideOffset.y);


	tileMapDispl = tileMapPos;
	movingX = false;
	movingY = false;
}

void setSign(int &value, char sign) {
	switch (sign) {
	case '-':	if (value > 0) value = -value; break;
	case '+':	if (value < 0) value = -value; break;
	}
}

int Player::update(int deltaTime)
{
 	movingX = false;
	movingY = false;

	glm::ivec2 slideLogic = slide->getLogicSize();

	if (deathAnimation)
	{
		if (animation < 5)
			animation = 5;
		else if (animation < 12)
			++animation;
		else {
			animation = 1;
			deathAnimation = false;
		}
	}
	else
	{
		switch (bonus)
		{
		case Bonus::multipleBall:
		case Bonus::wall:
			break;

		case Bonus::twix:
		{
			if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			{
				movingX = true;
				setSign(speedX, '+');
				if ((slideLogic.x == sizePlayer.x)) {
					if ((posPlayer.x + sizePlayer.x + speedX) > (map->getMapSizeX() * map->getTileSize() - map->getTileSize())) {
						movingX = false;
					}
				}
				else if ((posPlayer.x + sizePlayer.x + slideOffset.x + speedX) > ((map->getMapSizeX()) * map->getTileSize() - map->getTileSize())) {
					movingX = false;
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			{
				movingX = true;
				setSign(speedX, '-');

				if ((slideLogic.x == sizePlayer.x)) {
					if ((posPlayer.x + speedX) < map->getTileSize()) {
						movingX = false;
					}
				}
				else if ((posPlayer.x - slideOffset.x + speedX) < map->getTileSize()) {
					movingX = false;
				}

			}

			if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
			{
				int limit = map->getTileSize() * 2;
				int pos2Check = (int)posPlayer.y - slideOffset.y - slide->getLogicSize().y;
				int displacement = map->getMapSizeY() * map->getTileSize() * (3 - currRoom);

				movingY = true;
				setSign(speedY, '-');

				if (pos2Check < (limit + displacement)) {
					movingY = false;
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				int tilesize = map->getTileSize();
				int mapsize = map->getMapSizeY() * tilesize;
				int base_pixel = ((int)posPlayer.y + sizePlayer.y) % mapsize;

				movingY = true;
				setSign(speedY, '+');

				if (base_pixel < tilesize) {
					movingY = false;
				}
			}

			if (movingX) posPlayer.x += speedX;
			if (movingY) posPlayer.y += speedY;
			break;
		}
		case Bonus::blaster:
		case Bonus::doubleSlide:
		case Bonus::magnet:
		case Bonus::doublePoints:
		default:
		{
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			{
				movingX = true;
				setSign(speedX, '+');
				if ((slideLogic.x == sizePlayer.x)) {
					if ((posPlayer.x + sizePlayer.x + speedX) > (map->getMapSizeX() * map->getTileSize() - map->getTileSize())) {
						movingX = false;
					}
				}
				else if ((posPlayer.x + sizePlayer.x + slideOffset.x + speedX) > ((map->getMapSizeX()) * map->getTileSize() - map->getTileSize())) {
					movingX = false;
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			{
				movingX = true;
				setSign(speedX, '-');

				if ((slideLogic.x == sizePlayer.x)) {
					if ((posPlayer.x + speedX) < map->getTileSize()) {
						movingX = false;
					}
				}
				else if ((posPlayer.x - slideOffset.x + speedX) < map->getTileSize()) {
					movingX = false;
				}

			}

			if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				int limit = map->getTileSize() * 2;
				int pos2Check = (int)posPlayer.y - slideOffset.y - slide->getLogicSize().y;
				int displacement = map->getMapSizeY() * map->getTileSize() * (3 - currRoom);

				movingY = true;
				setSign(speedY, '-');

				if (pos2Check < (limit + displacement)) {
					movingY = false;
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
			{
				int tilesize = map->getTileSize();
				int mapsize = map->getMapSizeY() * tilesize;
				int base_pixel = ((int)posPlayer.y + sizePlayer.y) % mapsize;

				movingY = true;
				setSign(speedY, '+');

				if (base_pixel < tilesize) {
					movingY = false;
				}
			}

			if (movingX) posPlayer.x += speedX;
			if (movingY) posPlayer.y += speedY;
		}
		
		}

			
		
	}
		

	updateEyesAnimation();

	posPlayer = glm::mod(posPlayer, glm::vec2(1000.f, 192.f)) - glm::vec2(0.f, float(tiles_displacement) * 8.f);
	sprite->changeAnimation(animation);
	sprite->setPosition((glm::vec2) tileMapDispl + posPlayer);
	slide->setPosition((glm::vec2) tileMapDispl - (glm::vec2) slideOffset + posPlayer);

	if (movingX && speedX > 0) return right;

	else if (movingX && speedX < 0) return left;

	else if (movingY && speedY < 0) return up;

	else if (movingY && speedY > 0) return down;

	else if (!movingX && !movingY) return still;

	return diag;
}

void Player::render(glm::mat4& displacement_mat)
{
	slide->render(displacement_mat);
	sprite->render(displacement_mat);
}

glm::ivec2 Player::getSize()
{
	return sizePlayer;
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = glm::mod(pos, glm::vec2(1000.f, 192.f)) - glm::vec2(0.f, float(tiles_displacement) * 8.f);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::setTilesDisplacement(int t)
{
	tiles_displacement = t;
}

void Player::setDeathAnimation(bool b)
{
	deathAnimation = b;
}

bool Player::getDeathAnimation()
{
	return deathAnimation;
}

glm::vec2 Player::getPosition()
{
	return posPlayer;
}

glm::ivec2 Player::getSlideOffset()
{
	return slideOffset;
}

float Player::getSpeedX()
{
	return speedX;
}

int Player::getBonus()
{
	return bonus;
}

int Player::getCurrentRoom()
{
	return currRoom;
}

void Player::setRoom(int room)
{
	currRoom = room;
}

void Player::updateEyesAnimation()
{
	if (deathAnimation) return;

	if (bonus == Bonus::twix) {
		animation = 0;
		return;
	}

	if (posMainBall.y + 48 < posPlayer.y) {
		animation = 2;
		return;
	}

	if (posMainBall.x + 10 < posPlayer.x) {
		animation = 3;
		return;
	}

	if (posMainBall.x - 10 > posPlayer.x + sizePlayer.x) {
		animation = 4;
		return;
	}

	animation = 1;

}

void Player::toogleChangeBar()
{
	slide->toogleChangeBar();
}

void Player::setBonus(int b)
{
	bonus = b;
	slide->setBonus(b);
}

void Player::setPosMainBall(glm::vec2 pos)
{
	posMainBall = pos;
}

bool Player::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, float* posI, int speed, float &modifierY, float& modifierX)
{


	if (slide->collisionMoveDown(pos, size, posI, speed, modifierY, modifierX)) return true;

	bool collision = false;
	glm::vec2 originalSlidePos((glm::vec2) tileMapDispl - (glm::vec2) slideOffset + posPlayer);


	if (movingX && movingY) {
		
		int total_speed = std::abs(speedX) + std::abs(speedY);
		int sx = speedX / std::abs(speedX);
		int sy = speedY / std::abs(speedY);

		for (int k = 1, x = 0, y = 0; !collision && k < total_speed; ++k) {
			
			if (k % 2)	x += sx;
			else		y += sy;

			slide->setPosition((glm::vec2) tileMapDispl - (glm::vec2) slideOffset + posPlayer + glm::vec2(x,y));
			collision = slide->collisionMoveDown(pos, size, posI, speed, modifierY, modifierX);
		}
	}
	else if (movingX && !movingY) {
		
		int total_speed = std::abs(speedX);
		int sx = speedX / std::abs(speedX);
		int sy = 0;

		for (int k = 1, i = 0, j = 0; !collision && k < total_speed; ++k) {

			j += sx;

			slide->setPosition((glm::vec2) tileMapDispl - (glm::vec2) slideOffset + posPlayer + glm::vec2(sx*k, 0));
			collision = slide->collisionMoveDown(pos, size, posI, speed, modifierY, modifierX);
		}
	}
	else if (!movingX && movingY) {

		int total_speed = std::abs(speedY);
		int sx = 0;
		int sy = speedY / std::abs(speedY);

		for (int k = 1; !collision && k < total_speed; ++k) {

			slide->setPosition((glm::vec2) tileMapDispl - (glm::vec2) slideOffset + posPlayer + glm::vec2(0, sy*k));
			collision = slide->collisionMoveDown(pos, size, posI, speed, modifierY, modifierX);
		}
	}

	slide->setPosition(originalSlidePos);

	return collision;
}

bool Player::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size, float* posJ, int speed)
{
	if (slide->collisionMoveRight(pos, size, posJ, speed)) return true;

	bool collision = false;
	glm::vec2 originalSlidePos((glm::vec2) tileMapDispl - (glm::vec2) slideOffset + posPlayer);


	if (movingX && movingY) {

		int total_speed = std::abs(speedX) + std::abs(speedY);
		int sx = speedX / std::abs(speedX);
		int sy = speedY / std::abs(speedY);

		for (int k = 1, x = 0, y = 0; !collision && k < total_speed; ++k) {

			if (k % 2)	x += sx;
			else		y += sy;

			slide->setPosition((glm::vec2) tileMapDispl - (glm::vec2) slideOffset + posPlayer + glm::vec2(x, y));
			collision = slide->collisionMoveRight(pos, size, posJ, speed);
		}
	}
	else if (movingX && !movingY) {

		int total_speed = std::abs(speedX);
		int sx = speedX / std::abs(speedX);
		int sy = 0;

		for (int k = 1, i = 0, j = 0; !collision && k < total_speed; ++k) {

			j += sx;

			slide->setPosition((glm::vec2) tileMapDispl - (glm::vec2) slideOffset + posPlayer + glm::vec2(sx * k, 0));
			collision = slide->collisionMoveRight(pos, size, posJ, speed);
		}
	}
	else if (!movingX && movingY) {

		int total_speed = std::abs(speedY);
		int sx = 0;
		int sy = speedY / std::abs(speedY);

		for (int k = 1; !collision && k < total_speed; ++k) {

			slide->setPosition((glm::vec2) tileMapDispl - (glm::vec2) slideOffset + posPlayer + glm::vec2(0, sy * k));
			collision = slide->collisionMoveRight(pos, size, posJ, speed);
		}
	}

	slide->setPosition(originalSlidePos);

	return collision;

}

bool Player::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, float* posJ, int speed)
{

	if (slide->collisionMoveLeft(pos, size, posJ, speed)) return true;

	bool collision = false;
	glm::vec2 originalSlidePos((glm::vec2) tileMapDispl - (glm::vec2) slideOffset + posPlayer);


	if (movingX && movingY) {

		int total_speed = std::abs(speedX) + std::abs(speedY);
		int sx = speedX / std::abs(speedX);
		int sy = speedY / std::abs(speedY);

		for (int k = 1, x = 0, y = 0; !collision && k < total_speed; ++k) {

			if (k % 2)	x += sx;
			else		y += sy;

			slide->setPosition((glm::vec2) tileMapDispl - (glm::vec2) slideOffset + posPlayer + glm::vec2(x, y));
			collision = slide->collisionMoveLeft(pos, size, posJ, speed);
		}
	}
	else if (movingX && !movingY) {

		int total_speed = std::abs(speedX);
		int sx = speedX / std::abs(speedX);
		int sy = 0;

		for (int k = 1, i = 0, j = 0; !collision && k < total_speed; ++k) {

			j += sx;

			slide->setPosition((glm::vec2) tileMapDispl - (glm::vec2) slideOffset + posPlayer + glm::vec2(sx * k, 0));
			collision = slide->collisionMoveLeft(pos, size, posJ, speed);
		}
	}
	else if (!movingX && movingY) {

		int total_speed = std::abs(speedY);
		int sx = 0;
		int sy = speedY / std::abs(speedY);

		for (int k = 1; !collision && k < total_speed; ++k) {

			slide->setPosition((glm::vec2) tileMapDispl - (glm::vec2) slideOffset + posPlayer + glm::vec2(0, sy * k));
			collision = slide->collisionMoveLeft(pos, size, posJ, speed);
		}
	}

	slide->setPosition(originalSlidePos);

	return collision;
}
