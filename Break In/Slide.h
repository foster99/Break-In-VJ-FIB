#ifndef _SLIDE_INCLUDE
#define _SLIDE_INCLUDE

#include "Sprite.h"

class Slide
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int sizeX);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2& pos);
private:
	glm::ivec2 tileMapDispl, logicSize, quadSize, posSlide;
	Texture tex;
	Sprite* sprite;
};
#endif
