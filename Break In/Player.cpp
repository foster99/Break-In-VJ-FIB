#include "Player.h"
#include "Game.h"


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{	
	
	sizePlayer = glm::ivec2(19,16);
	displ_posPlayer = mod(posPlayer, glm::vec2(24.f));
	speedX = 3;
	speedY = 2;
	spritesheet.loadFromFile("images/eyes.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizePlayer, glm::vec2(0.33f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(12);
	
	sprite->addKeyframe(0,	glm::vec2(0.f,		0.f));
	sprite->addKeyframe(1,	glm::vec2(0.33f,	0.f));
	sprite->addKeyframe(2,	glm::vec2(0.66f,	0.f));
	sprite->addKeyframe(3,	glm::vec2(0.f,		0.25f));
	sprite->addKeyframe(4,	glm::vec2(0.33f,	0.25f));
	sprite->addKeyframe(5,	glm::vec2(0.66f,	0.25f));
	sprite->addKeyframe(6,	glm::vec2(0.f,		0.5f));
	sprite->addKeyframe(7,	glm::vec2(0.33f,	0.5f));
	sprite->addKeyframe(8,	glm::vec2(0.66f,	0.5f));
	sprite->addKeyframe(9,	glm::vec2(0.f,		0.75f));
	sprite->addKeyframe(10, glm::vec2(0.33f,	0.75f));
	sprite->addKeyframe(11, glm::vec2(0.66f,	0.75f));



	sprite->changeAnimation(4);

	slideOffsetX = 10;
	slideOffsetY = 6;

	slide = new Slide();
	slide->init(tileMapPos, shaderProgram);
	slide->setPosition(glm::ivec2(posPlayer.x - slideOffsetX, posPlayer.y- slideOffsetY));


	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + displ_posPlayer.x), float(tileMapDispl.y + displ_posPlayer.y)));
	
}

void Player::update(int deltaTime)
{

	glm::ivec2 slideLogic = slide->getLogicSize();

	if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		posPlayer.x += speedX;
		if (slideLogic.x == sizePlayer.x) {
			if ((posPlayer.x + sizePlayer.x) > ((map->getMapSizeX()) * map->getTileSize() - map->getTileSize()))
				posPlayer.x -= speedX;
		}
		else {
			if ((posPlayer.x + sizePlayer.x + slideOffsetX) > ((map->getMapSizeX()) * map->getTileSize() - map->getTileSize()))
				posPlayer.x -= speedX;
		}
		
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		posPlayer.x -= speedX;
		if (slideLogic.x == sizePlayer.x) {
			if (posPlayer.x < map->getTileSize())
				posPlayer.x += speedX;
		}
		else {
			if ((posPlayer.x - slideOffsetX +1) < map->getTileSize())
				posPlayer.x += speedX;
		}
	}

	if (Game::instance().getSpecialKey(GLUT_KEY_UP))
	{
		posPlayer.y -= speedY;
		if (posPlayer.y < (map->getTileSize()*2))
			posPlayer.y += speedY;
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
	{
		posPlayer.y += speedY;
		if ((posPlayer.y + sizePlayer.x) > ( (map->getMapSizeY()-1)*map->getTileSize()) )
			posPlayer.y -= speedY;
	}

	displ_posPlayer = mod(posPlayer, glm::vec2(8.f * 24.f));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + displ_posPlayer.x), float(tileMapDispl.y + displ_posPlayer.y)));
	slide->setPosition(glm::ivec2(displ_posPlayer.x-slideOffsetX, displ_posPlayer.y - slideOffsetY));

}

void Player::render()
{
	sprite->render();
	slide->render();
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
	posPlayer = pos;
	displ_posPlayer = mod(posPlayer, glm::vec2(8.f * 24.f));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




