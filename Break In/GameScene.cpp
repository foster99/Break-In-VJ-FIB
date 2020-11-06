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

	if (player != NULL)
		delete player;
	if (ball != NULL)
		delete ball;
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
	else if (ballOnDoor())							// COMPROBAR SI HAY QUE CAMBIAR DE ROOM
	{
		if (room_old < room)		stride = +1;
		else /*(room_old > currRoom)*/	stride = -1;

		switch (room) {
		case 1: end = -48;	break;
		case 2: end = -24;	break;
		case 3: end = 0;	break;
		}
	}
	else {
	
		bool ballCollided = ball->update(deltaTime);
		// PILOTA REOTRNA POS EN TILES D'ON IMPACTE glm::ivec2 collisionIn(i,j)
		
		if (ballCollided)
		{
			money += 100;
			menuMap->setMoney(money);

			points += 33;
			menuMap->setPoints(points);

			if (points > 1000) {
				menuMap->setLine("AII LMAO", "4POGGERS"); // CAMBIA CON EL BONUS
			}
		}

		// He col
		glm::ivec2 last_collision_coords = glm::ivec2(0);
		if (godMode)
		{

			map->deleteKey(60, 2);
			godMode = false;

			// SOLO DESAPARESE LA LLAVE CUANDO SE UPDATEA LA PARTE DINAMICA
			// LA PARTE DINAMICA SE UPDATEA CUANDO LA PELOTA 
		}

		map->setRoom(room);
		player->setRoom(room);
		player->setTilesDisplacement(tiles_displacement);
		player->setPosMainBall(ball->getPosition());
	}

	if (bonus->update(deltaTime))
		player->setBonus(bonus->getActiveBonus());

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

	if (lives < 0) gameIsOver();
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

bool GameScene::ballOnDoor()
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

bool GameScene::lastBallisDead()
{
	return map->tileIsDeath((ball->getBasePositionInTiles()).y, (ball->getBasePositionInTiles()).x);
}

void GameScene::startBank()
{
	string path;
	if (bank < 10)	path = "levels/BANK_0" + to_string(bank) +"_test.txt";
	else			path = "levels/BANK_" + to_string(bank) + "_test.txt";
	
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
	bonus->setPlayer(player);
}

bool GameScene::getGameOver()
{
	return gameOver;
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