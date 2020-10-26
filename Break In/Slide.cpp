#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Slide.h"

void Slide::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int sizeX)
{
	logicSize = glm::ivec2(sizeX, 6);
	quadSize = glm::ivec2(20, 6);

	tex.loadFromFile("images/bars.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(quadSize, glm::vec2(1.f, 0.2f), &tex, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(1, glm::vec2(0.f, 0.2f));
	sprite->addKeyframe(2, glm::vec2(0.f, 0.4f));
	sprite->addKeyframe(3, glm::vec2(0.f, 0.6f));
	sprite->addKeyframe(4, glm::vec2(0.f, 0.8f));

	sprite->changeAnimation(2);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSlide.x), float(tileMapDispl.y + posSlide.y)));


}

void Slide::update(int deltaTime)
{
}

void Slide::render()
{
	sprite->render();
}

void Slide::setPosition(const glm::vec2& pos)
{
	posSlide = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSlide.x), float(tileMapDispl.y + posSlide.y)));
}
