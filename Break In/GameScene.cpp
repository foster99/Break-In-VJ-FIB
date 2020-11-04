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
	map = TileMap::createTileMap("levels/BANK_01_test.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	

	menuMap = MenuTileMap::createTileMap("levels/menu.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	menuMap->setBank(bank);
	menuMap->setRoom(room);
	menuMap->setLives(lives);
	menuMap->setMoney(money);
	menuMap->setPoints(points);
	menuMap->setLine(" CASUAL ", " PLAYER ");


	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTilesDisplacement(0);
	player->setRoom(room);
	player->setTileMap(map);

	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	ball->setTileMap(map);
	ball->setPlayer(player);
}

void GameScene::update(int deltaTime) {

	this->Scene::update(deltaTime);

	if (!alive) {
		auxTime += deltaTime;
		if (auxTime > 175.f) {
			auxTime = 0;
			player->update(deltaTime);
		}
		return;
	}
	else if (scrolling)
	{
		tiles_displacement += stride;
		scrolling = tiles_displacement != end;
	}
	else if (ballOnDoor())
	{
		if (room_old < room)		stride = +1;
		else /*(room_old > room)*/	stride = -1;

		switch (room) {
		case 1: end = -48;	break;
		case 2: end = -24;	break;
		case 3: end = 0;	break;
		}
	}
	else if (lastBallisDead())
	{
		playerLosesLife();
		player->update(deltaTime);
	}
	else
	{
		player->update(deltaTime);

		// PILOTA REOTRNA POS EN TILES D'ON IMPACTE glm::ivec2 collisionIn(i,j)
		if (ball->update(deltaTime))
		{
			money += 100;
			menuMap->setMoney(money);

			points += 33;
			menuMap->setPoints(points);

			lives += 1;
			menuMap->setLives(lives);

			if (points > 1000) {
				menuMap->setLine("AII LMAO", "4POGGERS"); // CAMBIA CON EL BONUS
			}

			map->prepareDynamicArrays();
			//map->prepareStaticArrays();
		}

		// Test del borrado de llaves
		glm::ivec2 last_collision_coords = glm::ivec2(0);
		if (godMode)
		{

			map->deleteKey(60, 2);
			godMode = false;

			// SOLO DESAPARESE LA LLAVE CUANDO SE UPDATEA LA PARTE DINAMICA
			// LA PARTE DINAMICA SE UPDATEA CUANDO LA PELOTA COLISIONA


		}

		player->setRoom(room);
		player->setTilesDisplacement(tiles_displacement);
	}

	displacement_mat = glm::translate(glm::mat4(1.f), glm::vec3(0.f, float(tiles_displacement *8), 0.f));
}

void GameScene::render()
{
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", displacement_mat);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	
	map->render();
	if (!scrolling) player->render(displacement_mat);
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

	if (lives < 0) gameOver();
	else {
		menuMap->setLives(lives);
		player->setDeathAnimation(true);
		Game::instance().playLoseLiveSound();
	}
}

void GameScene::gameOver()
{
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

void GameScene::toggleGodMode()
{
	godMode = !godMode;
	// possible update ????
}