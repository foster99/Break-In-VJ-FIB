#include "StartMenuScene.h"
#include "Game.h"
#include <sstream>
#include <iomanip>

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

	pointsCoords = glm::vec3(8.f * 19.f, 8.f * 20.f + 5.f, 10.f);
	setPoints(0);
	digits.loadFromFile("images/digits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	pointsQuads = vector<Sprite*>(int(pointsCoords.z), NULL);
	prepareCounters(pointsQuads, pointsCoords, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
}

void StartMenuScene::update(int deltaTime)
{
	this->Scene::update(deltaTime);

	for (int digit = 0; (unsigned int)digit < pointsQuads.size(); ++digit) {

		Sprite*& sprite = pointsQuads[int(pointsCoords.z) - 1 - digit];
		sprite->changeAnimation(points[digit] - '0');
	}
}

void StartMenuScene::render()
{
	if (glm::mod(currentTime,1500.f) < 750.f)	texQuad->render(tex1);
	else										texQuad->render(tex2);

	renderPoints();

	this->Scene::render();
}

void StartMenuScene::changeTex()
{
	// actTexture = (actTexture + 1) % 2; // REMOVE
}

string StartMenuScene::intToStringOfNDigits(int i, int Ndigits)
{
	std::stringstream ss;
	ss << std::setw(Ndigits) << std::setfill('0') << i;
	string s = ss.str();

	/*if (s.length() > unsigned int(Ndigits))
		throw "EL NUMERO TIENE MAS DIGITOS DE LOS SOLICITADOS!!";*/

	return s;
}

void StartMenuScene::updatePoints()
{
	for (int digit = 0; (unsigned int)digit < pointsQuads.size(); ++digit) {

		Sprite*& sprite = pointsQuads[pointsCoords.z - 1 - digit];
		sprite->changeAnimation(points[digit] - '0');
	}
}

void StartMenuScene::renderPoints()
{
	for (auto& digit : pointsQuads) digit->render();

}

void StartMenuScene::setPoints(int p)
{
	points = intToStringOfNDigits(p, int(pointsCoords.z));
}

void StartMenuScene::prepareCounters(vector<Sprite*>& quads, glm::vec3& coords, const glm::vec2& minCoords, ShaderProgram& program)
{
	float tilesize = 8.f;

	for (int digit = 0; (unsigned int)digit < quads.size(); ++digit) {

		Sprite*& sprite = quads[digit];
		sprite = Sprite::createSprite(glm::ivec2(tilesize), glm::vec2(1.f / 10.f, 1.f), &digits, &program);
		sprite->setNumberAnimations(10);

		sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(1, glm::vec2(1.f / 10.f, 0.f));
		sprite->addKeyframe(2, glm::vec2(2.f / 10.f, 0.f));
		sprite->addKeyframe(3, glm::vec2(3.f / 10.f, 0.f));
		sprite->addKeyframe(4, glm::vec2(4.f / 10.f, 0.f));
		sprite->addKeyframe(5, glm::vec2(5.f / 10.f, 0.f));
		sprite->addKeyframe(6, glm::vec2(6.f / 10.f, 0.f));
		sprite->addKeyframe(7, glm::vec2(7.f / 10.f, 0.f));
		sprite->addKeyframe(8, glm::vec2(8.f / 10.f, 0.f));
		sprite->addKeyframe(9, glm::vec2(9.f / 10.f, 0.f));

		sprite->changeAnimation(1);

		sprite->setPosition(glm::vec2(minCoords.x + coords.x + (coords.z - 1.f - float(digit)) * tilesize,
									  minCoords.y + coords.y
							));
	}

}
