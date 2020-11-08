#include "Game.h"
#include "GameScene.h"
#include "MenuTileMap.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

GameScene::GameScene()
{
	player = NULL;
}

GameScene::~GameScene()
{
	this->Scene::~Scene();

	if (player   != NULL)	delete player;
	if (ball     != NULL)	delete ball;
	if (guardian != NULL)	delete guardian;
	if (bonus    != NULL)	delete bonus;
}

void GameScene::init() {

	this->Scene::init();

	// Initializations
	room = 1;
	room_old = 1;
	lives = 3;
	scrolling = false;
	bank = 1;
	tiles_displacement = -48;
	stride = 0;
	end = 0;
	godMode = false;
	alive = true;
	gameOver = false;
	win = false;
}

void GameScene::update(int deltaTime) {

	this->Scene::update(deltaTime);

	if (!alive) {									// ESTAMOS MUERTOS
		auxTime += deltaTime;
		if (auxTime > 175.f) {
			auxTime = 0;
			player->update(deltaTime);
		}
		alive = !(player->getDeathAnimation());

		if (alive) restartPlayerBall();
		else       return;
	}
	else if (lastBallisDead())						// COMPROBAR SI HEMOS MUERTO 
	{
		playerLosesLife();
		player->update(deltaTime);
	}
	else if (scrolling)								// ESTAMOS REALIZANDO SCROLL ENTRE ROOMS
	{
		tiles_displacement += stride;
		scrolling = tiles_displacement != end;
		bonus->restartTime();
	}
	else if (changeOfRoom())						// COMPROBAR SI HAY QUE CAMBIAR DE ROOM
	{
		if (room_old < room)			stride = +1;
		else /*(room_old > currRoom)*/	stride = -1;

		switch (room) {
		case 1: end = -48;	break;
		case 2: end = -24;	break;
		case 3: end = 0;	break;
		}
	}
	else {
		
		if (ball->update(deltaTime))
		{
			glm::ivec2 tile = ball->getLastCollision();
			
			if (tile.x < 0 || tile.y < 0) {
				Game::instance().playPlayerSound();
			}
			else {
				switch (map->tileCollision(tile[0], tile[1]))
				{
				case Tile::moneyBag:
					Game::instance().playMoneySound();
					money += 150;
					points += 200;
					break;

				case Tile::coin:
					Game::instance().playBrickSound();
					money += 50;
					points += 125;
					break;

				case Tile::blueSpheres:
					Game::instance().playMoneySound();
					money += 300;
					points += 333;
					break;

				case Tile::brickBlue:
				case Tile::brickRed:
				case Tile::brickYellow:
				case Tile::brickGreen:
				case Tile::brickLow:
					Game::instance().playBrickSound();
					points += 100;
					break;

				case Tile::outCard:
					Game::instance().playGreenCardSound();
					points += 100;

				case Tile::alarm:
					points += 100;
					guardian->alarmOn();

				default: break;
				}
			}
			
			menuMap->setMoney(money);
			menuMap->setPoints(points);

			if (points > 1000) {
				menuMap->setLine("AII LMAO", "4POGGERS"); // CAMBIA CON EL BONUS
			}

		}

		map->setRoom(room);
		player->setRoom(room);
		player->setTilesDisplacement(tiles_displacement);
		player->setPosMainBall(ball->getPosition());
	}

	if (bonus->update(deltaTime))
		player->setBonus(bonus->getActiveBonus());

	if (guardian->getRoom() == player->getCurrentRoom()) {
		if (guardian->update(deltaTime))
			playerLosesLife();
	}
	else guardian->restartTime();

	player->update(deltaTime);
	map->prepareDynamicArrays();
	displacement_mat = glm::translate(glm::mat4(1.f), glm::vec3(0.f, float(tiles_displacement * 8), 0.f));
}

void GameScene::render()
{
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", displacement_mat);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	
	map->render();
	if (!scrolling)		player->render(displacement_mat);
	
	bonus->render(displacement_mat);
	guardian->render(displacement_mat);
	ball->render(displacement_mat);

	// Render Lateral Menu
	glm::mat4 menu_modelview = glm::translate(glm::mat4(1.f), glm::vec3(192.f, 0.f, 0.f));
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", menu_modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	menuMap->render();
}

void GameScene::toogleChangeBar()
{
	player->toogleChangeBar();
}

void GameScene::playerLosesLife()
{
	lives -= 1;
	alive = false;

	if (lives < 0) {
		player->setDeathAnimation(true);
		Game::instance().playLoseLiveSound();
		gameIsOver();
	}
	else {
		menuMap->setLives(lives);
		player->setDeathAnimation(true);
		Game::instance().playLoseLiveSound();
	}
}

void GameScene::gameIsOver()
{
	gameOver = true;
	// print GameOver Quad
	// Set history points
	// return to Menu Scene
}

bool GameScene::changeOfRoom()
{
	room_old = room;



	if (ball->getPosition().y / 8 > 48) {
		room = 1;
		menuMap->setRoom(room);
	}
	else if (ball->getPosition().y / 8 > 24) {
		room = 2;
		menuMap->setRoom(room);
	}
	else {
		room = 3;
		menuMap->setRoom(room);
	}

	scrolling = room != room_old;

	return scrolling;
}

void GameScene::nextRoom()
{
	if (godMode && room < 3)
	{
		glm::vec2 ballPos = ball->getPosition();
		ballPos.y -= 24 * 8;
		ball->setPosition(ballPos);
	}
}

void GameScene::prevRoom()
{
	if (godMode && room > 1)
	{
		glm::vec2 ballPos = ball->getPosition();
		ballPos.y += 24 * 8;
		ball->setPosition(ballPos);
	}
}

bool GameScene::lastBallisDead()
{
	return map->tileIsDeath((ball->getBasePositionInTiles()).y, (ball->getBasePositionInTiles()).x);
}

void GameScene::startBank()
{
	string path;
	if (bank < 10)	path = "levels/BANK_0" + to_string(bank) +".txt";
	else			path = "levels/BANK_" + to_string(bank) + ".txt";
	
	map = TileMap::createTileMap(path.c_str(), glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map->setBank(bank);
	map->setRoom(room);

	menuMap = MenuTileMap::createTileMap("levels/menu.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	menuMap->setBank(bank);
	menuMap->setRoom(room);
	menuMap->setLives(lives);
	menuMap->setMoney(money);
	menuMap->setPoints(points);
	menuMap->setLine(" CASUAL ", " PLAYER ");

	restartPlayerBall();
}

void GameScene::restartPlayerBall()
{
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTilesDisplacement(tiles_displacement);
	player->setRoom(room);
	player->setTileMap(map);

	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setPosition(glm::vec2(INIT_BALL_X_TILES * map->getTileSize(), INIT_BALL_Y_TILES * map->getTileSize()));
	ball->setTileMap(map);
	ball->setPlayer(player);

	player->setPosMainBall(ball->getPosition());

	bonus = new Bonus();
	bonus->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	bonus->setTileMap(map);
	bonus->setPosition(glm::vec2(INIT_BONUS_X_TILES * map->getTileSize(), INIT_BONUS_Y_TILES * map->getTileSize()));

	guardian = new Guardian();
	guardian->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	guardian->setTileMap(map);
	guardian->setPosition(glm::vec2(INIT_BONUS_X_TILES * map->getTileSize(), INIT_BONUS_Y_TILES * map->getTileSize()));
	guardian->setPlayer(player);
	guardian->setRoom(map->getGuardianRoom());
}

bool GameScene::getGameOver()
{
	return !player->getDeathAnimation() && gameOver;
}

bool GameScene::getWin()
{
	return win;
}

int GameScene::getBank()
{
	return bank;
}

int GameScene::getRoom()
{
	return room;
}

int GameScene::getLives()
{
	return lives;
}

int GameScene::getMoney()
{
	return money;
}

int GameScene::getPoints()
{
	return points;
}

void GameScene::nextBank()
{
	// standart initializations
	room = 1;
	room_old = 1;
	tiles_displacement = -48;

	// next bank modifications
	++bank;
	alive = true;
	lives += 1;

	startBank();
}

void GameScene::setBank(int b)
{
	bank = b;
}

void GameScene::setRoom(int r)
{
	room = r;
}

void GameScene::setLives(int l)
{
	lives = l;
}

void GameScene::setMoney(int m)
{
	money = m;
}

void GameScene::setPoints(int p)
{
	points = p;
}

void GameScene::toggleGodMode()
{
	godMode = !godMode;
	// possible update ????
}