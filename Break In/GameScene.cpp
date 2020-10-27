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

	loopsToRender = 5;
	currLoop = 0;
	godMode = false;

	staticMap = new StaticTileMap("levels/no_path_test.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * staticMap->getTileSize(), INIT_PLAYER_Y_TILES * staticMap->getTileSize()));
	player->setTileMap(staticMap);

	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setPosition(glm::vec2(INIT_PLAYER_X_TILES * staticMap->getTileSize(), INIT_PLAYER_Y_TILES * staticMap->getTileSize()));
	ball->setTileMap(staticMap);
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

	staticMap->render();
	player->render();
	ball->render();
}

void GameScene::toggleGodMode()
{
	godMode = !godMode;
	// possible update ????
}

bool GameScene::itIsALoopToRender()
{
	if (loopsToRender <= currLoop++) {
		currLoop = 0;
		return true;
	}
	return false;
}
