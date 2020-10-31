#include "StartMenuScene.h"
#include "Game.h"

StartMenuScene::StartMenuScene()
{
	texQuad = NULL;
}

StartMenuScene::~StartMenuScene()
{
	if (texQuad != NULL) 
		delete texQuad;
}

void StartMenuScene::init()
{
	this->Scene::init();

	tex1.loadFromFile("images/title_screen_01.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tex2.loadFromFile("images/title_screen_02.png", TEXTURE_PIXEL_FORMAT_RGBA);
	glm::vec2 geom[2] = { glm::vec2(0.f,0.f),glm::vec2(Game::SCREEN_WIDTH,Game::SCREEN_HEIGHT) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f,0.f), glm::vec2(1.f,1.f) };
	texQuad = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
}

void StartMenuScene::update(int deltaTime)
{
	this->Scene::update(deltaTime);

	// Put menuScene updates HERE
}

void StartMenuScene::render()
{
	if (glm::mod(currentTime,1500.f) < 750.f)
		texQuad->render(tex1);
	else 
		texQuad->render(tex2);

	this->Scene::render();
}

void StartMenuScene::changeTex()
{
	// actTexture = (actTexture + 1) % 2; // REMOVE
}