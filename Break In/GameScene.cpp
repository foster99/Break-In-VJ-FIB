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

	tiles_displacement = 0;
	godMode = false;

	staticMap = new StaticTileMap("levels/no_path_test.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	menuMap = new StaticTileMap("levels/menu.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

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

	// Modify tiles displacement

	if (ball->getPosition().y / 8 > 48) {
		room = 1;
		tiles_displacement = -48;
	}
	else if (ball->getPosition().y / 8 > 24) {
		room = 2;
		tiles_displacement = -24;
	}
	else {
		room = 3;
		tiles_displacement = 0;
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

	staticMap->render();
	player->render();
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

void GameScene::toggleGodMode()
{
	godMode = !godMode;
	// possible update ????
}