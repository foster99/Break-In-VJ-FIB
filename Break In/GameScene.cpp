#include "GameScene.h"


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

	loopsToRender = 1;
	actLoop = 0;
	godMode = false;

	map = TileMap::createTileMap("levels/leveltest.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	ball->setTileMap(map);
}

void GameScene::update(int deltaTime) {

	this->Scene::update(deltaTime);
	player->update(deltaTime);
	ball->update(deltaTime);
	++actLoop;

}

void GameScene::render()
{
	if (loopsToRender == actLoop) {
		actLoop = 0;
		this->Scene::render();
		map->render();
		player->render();
		ball->render();
	}
}

void GameScene::toggleGodMode()
{
	godMode = !godMode;
	// possible update ????
}
