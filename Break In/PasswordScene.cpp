#include "PasswordScene.h"
#include "Game.h"

PasswordScene::PasswordScene()
{
	texQuad = NULL;
}

PasswordScene::~PasswordScene()
{
	if (texQuad != NULL) 
		delete texQuad;
}

void PasswordScene::init()
{
	this->Scene::init();
	
	//actTexture = 0; // REMOVE
	tex.loadFromFile("images/password_screen.png", TEXTURE_PIXEL_FORMAT_RGBA);
	glm::vec2 geom[2] = { glm::vec2(0.f,0.f),glm::vec2(Game::SCREEN_WIDTH,Game::SCREEN_HEIGHT) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f,0.f), glm::vec2(1.f,1.f) };
	texQuad = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
}

void PasswordScene::update(int deltaTime)
{
	this->Scene::update(deltaTime);

	// Put menuScene updates HERE
}

void PasswordScene::render()
{
	texQuad->render(tex);

	this->Scene::render();
}

void PasswordScene::changeTex()
{
	// actTexture = (actTexture + 1) % 2; // REMOVE
}