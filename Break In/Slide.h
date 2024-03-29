#ifndef _SLIDE_INCLUDE
#define _SLIDE_INCLUDE

#include "Sprite.h"

class Slide
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);

	void render(glm::mat4& displacement_mat);
	
	glm::ivec2 getLogicSize();

	void setPosition(const glm::vec2& pos);
	void setOffSets(int offX, int offY);
	void setBonus(int b);

	void toogleChangeBar();
	bool collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, float* posI, int speed, float& modifierY, float& modifierX);
	bool collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size, float* posJ, int speed);
	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, float* posJ, int speed);
private:
	bool onSlide(const glm::ivec2& pos, int sizeX, float& modifierY, float& modifierX);
	bool onSide(const glm::ivec2& pos, int sizeY);

	int bonus;
	glm::ivec2 tileMapDispl, logicSize, quadSize, posSlide, singleSize, offSet;
	glm::vec2 displ_posPlayer;
	Texture tex;
	Sprite* sprite = NULL;
};
#endif
