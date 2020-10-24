#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
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
	
	//actTexture = 0; // REMOVE
	tex.loadFromFile("images/title_screen.png", TEXTURE_PIXEL_FORMAT_RGBA);
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
	texQuad->render(tex);

	this->Scene::render();
	
	// REMOVE
	//switch (actTexture) {
	//	case 0: texQuad->render(tex);		break;
	//	case 1: texQuad->render(options_tex);	break;
	//}
}

void StartMenuScene::changeTex()
{
	// actTexture = (actTexture + 1) % 2; // REMOVE
}