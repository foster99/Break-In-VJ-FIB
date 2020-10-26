#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "GameScene.h"
#include "Bank.h"

class Ball
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setBank(Bank* b);
	void setPosition(const glm::vec2& pos);

private:
	
	Texture tex;
	Sprite* sprite;
	TileMap* map;
	Bank* bank;
	
	bool collisionMoveDown();
	bool collisionMoveRight();
	bool collisionMoveUp();
	bool collisionMoveLeft();

	int tileSize;

	glm::ivec2 tileMapDispl, direction;
	glm::vec2 posBall;
	int sizeBall;

	float speed;
	float spdModifierX;
	float spdModifierY;
};
#endif // _BALL_INCLUDE

