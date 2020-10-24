#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include "OptionsScene.h"
#include "Game.h"

OptionsScene::OptionsScene()
{
	texQuad = NULL;
}

OptionsScene::~OptionsScene()
{
	if (texQuad != NULL) 
		delete texQuad;
}

void OptionsScene::init()
{
	this->Scene::init();
	
	//actTexture = 0; // REMOVE
	texOptions.loadFromFile("images/options_screen.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texInstructions.loadFromFile("images/plane_colors.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texCredits.loadFromFile("images/bricks.png", TEXTURE_PIXEL_FORMAT_RGBA);
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
	switch (mode) {
	case Game::options:
		texQuad->render(texOptions);
		break;
	case Game::credits:
		texQuad->render(texCredits);
		break;
	case Game::instructions:
		texQuad->render(texInstructions);
		break;
	}

	this->Scene::render();
	

}

void OptionsScene::changeTex()
{
	// actTexture = (actTexture + 1) % 2; // REMOVE
}

void OptionsScene::setMode(Mode m)
{
	mode = m;
}
