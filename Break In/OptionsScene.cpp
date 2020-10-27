#include "OptionsScene.h"
#include "Game.h"


OptionsScene::OptionsScene()
{
	texQuad = NULL;
}

OptionsScene::OptionsScene(string filename)
{
	texQuad = NULL;
	setTexture(filename);
}

OptionsScene::~OptionsScene()
{
	if (texQuad != NULL) 
		delete texQuad;
}

void OptionsScene::init()
{
	this->Scene::init();
	glm::vec2 geom[2] = { glm::vec2(0.f,0.f),glm::vec2(Game::SCREEN_WIDTH,Game::SCREEN_HEIGHT) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f,0.f), glm::vec2(1.f,1.f) };
	texQuad = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
}

void OptionsScene::update(int deltaTime)
{
	this->Scene::update(deltaTime);

	// Put menuScene updates HERE
}

void OptionsScene::render()
{
	texQuad->render(tex);
	this->Scene::render();
}

void OptionsScene::changeTex()
{
	// actTexture = (actTexture + 1) % 2; // REMOVE
}

void OptionsScene::setTexture(string filename)
{
	tex.loadFromFile(filename.c_str(), TEXTURE_PIXEL_FORMAT_RGBA);
}
