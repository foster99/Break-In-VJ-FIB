#include "Game.h"
#include "GameScene.h"
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

	godMode = false;
	level = 1;
	room = 1;

	bank = new Bank(level);
	//bank->setGameScene(this);
	bank->setStaticMap(new StaticTileMap(bank->getFilePath(), glm::vec2(SCREEN_X, SCREEN_Y), texProgram));
	//bank->setDynamicMap(new DynamicTileMap(bank->getFilePath(), glm::vec2(SCREEN_X, SCREEN_Y), texProgram));
	bank->init();

	player = new Player();
	player->setTileMap(map);
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));

	ball = new Ball();
	ball->setBank(bank);
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
}

void GameScene::update(int deltaTime) {

	this->Scene::update(deltaTime);
	player->update(deltaTime);
	ball->update(deltaTime);
}

void GameScene::render()
{
	glm::mat4 modelview = glm::mat4(1.f);
	modelview = glm::translate(modelview, glm::vec3(0.f, float(0.f), 0.f));
	// creo que la solucion es una matriz de traslacion o algo para mirar mas abajo
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	map->render();
	player->render();
	ball->render();
}

void GameScene::toggleGodMode()
{
	godMode = !godMode;
	// possible update ????
}
