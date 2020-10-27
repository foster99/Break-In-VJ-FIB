#include "Player.h"
#include "Game.h"


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	sizePlayer = 16;
	speedX = 3;
	speedY = 2;
	spritesheet.loadFromFile("images/bee.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(sizePlayer, sizePlayer), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
		
	sprite->changeAnimation(0);

	slideOffset = 6;

	slide = new Slide();
	slide->init(tileMapPos, shaderProgram, sizePlayer);
	slide->setPosition(glm::ivec2(posPlayer.x,posPlayer.y- slideOffset));


	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		posPlayer.x += speedX;
		if ((posPlayer.x + sizePlayer)> ( (map->getMapSizeX() - 9)*map->getTileSize()))
			posPlayer.x -= speedX;
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		posPlayer.x -= speedX;
		if ( posPlayer.x < map->getTileSize() )
			posPlayer.x += speedX;
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
		if ((posPlayer.y + sizePlayer) > ( (map->getMapSizeY()-1)*map->getTileSize()) )
			posPlayer.y -= speedY;
	}


	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	slide->setPosition(glm::ivec2(posPlayer.x, posPlayer.y - slideOffset));

}

void Player::render()
{
	sprite->render();
	slide->render();
}

int Player::getSize()
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
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




