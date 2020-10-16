#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include "MenuScene.h"
#include "Game.h"

MenuScene::MenuScene()
{
	texQuad = NULL;
}

MenuScene::~MenuScene()
{
	if (texQuad != NULL) 
		delete texQuad;
}

void MenuScene::init()
{
	glm::vec2 geom[2] = { glm::vec2(0.f,0.f),glm::vec2(640.f,480.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f,0.f), glm::vec2(1.f,1.f) };
	actTexture = 0;

	initShaders();
	texQuad = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	tex[0].loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tex[1].loadFromFile("images/opciones.png", TEXTURE_PIXEL_FORMAT_RGBA);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH-1), float(SCREEN_HEIGHT-1), 0.f);
	currentTime = 0.0f;
}

void MenuScene::update(int deltaTime)
{
	currentTime += deltaTime;
}

void MenuScene::render()
{
	glm::mat4 modelview = glm::mat4(1.0f);

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);

	texQuad->render(tex[actTexture]);
}

void MenuScene::changeTex()
{
	actTexture = (actTexture + 1) % 2;
}

void MenuScene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
}
