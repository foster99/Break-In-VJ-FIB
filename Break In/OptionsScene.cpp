#include "OptionsScene.h"
#include "Game.h"


OptionsScene::OptionsScene()
{
	texQuad = NULL;
}

OptionsScene::OptionsScene(int n)
{
	texQuad = NULL;
	setNTextures(n);
}

OptionsScene::~OptionsScene()
{
	if (texQuad != NULL) 
		delete texQuad;
}

void OptionsScene::init()
{
	this->Scene::init();
	currTexture = 0;
	tex = vector<Texture>(1);
	glm::vec2 geom[2] = { glm::vec2(0.f,0.f), glm::vec2(Game::SCREEN_WIDTH,Game::SCREEN_HEIGHT) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f,0.f), glm::vec2(1.f,1.f) };
	texQuad = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
}

void OptionsScene::update(int deltaTime)
{
	this->Scene::update(deltaTime);
}

void OptionsScene::render()
{
	texQuad->render(tex[currTexture]);
	this->Scene::render();
}

void OptionsScene::setNTextures(int n)
{
	tex = vector<Texture>(n);
}

void OptionsScene::nextTexture()
{
	currTexture = (currTexture + 1) % tex.size();
}

void OptionsScene::prevTexture()
{
	currTexture--;
	if (currTexture < 0) currTexture += tex.size();
}

int OptionsScene::getCurrTex() {
	return currTexture;
}

void OptionsScene::setTexture(int i, string filename)
{
	tex[i].loadFromFile(filename.c_str(), TEXTURE_PIXEL_FORMAT_RGBA);
}
