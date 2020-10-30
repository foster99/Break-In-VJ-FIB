#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "player.h"

class Ball
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void changeModifierX(float value);
	void changeModifierY(float value);
	void setTileMap(TileMap* tileMap);
	void setPlayer(Player* pla);
	void setPosition(const glm::vec2& pos);

private:
	glm::ivec2 tileMapDispl, direction;
	glm::vec2 posBall;
	Texture tex;
	Sprite* sprite;
	TileMap* map;
	Player* player;
	int sizeBall;
	float speed;
	float spdModifierX;
	float spdModifierY;
};

#endif // _BALL_INCLUDE

