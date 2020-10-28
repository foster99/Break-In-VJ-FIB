#ifndef _SLIDE_INCLUDE
#define _SLIDE_INCLUDE

#include "Sprite.h"

class Slide
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();
	
	glm::ivec2 getLogicSize();
	void setPosition(const glm::vec2& pos);
	void toogleChangeBar();
private:
	glm::ivec2 tileMapDispl, logicSize, quadSize, posSlide, singleSize;
	Texture tex;
	Sprite* sprite;
};
#endif