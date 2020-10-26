#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include <vector>

class Ball
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void changeModifierX(float value);
	void changeModifierY(float value);
	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	vector<glm::vec2> computePotentials(glm::vec2 p);
	void generateListOfTiles(glm::vec2 A, glm::vec2 B);

private:
	glm::ivec2 tileMapDispl, direction;
	glm::vec2 posBall;
	Texture tex;
	Sprite* sprite;
	TileMap* map;
	int sizeBall;

	float speed;
	float dirX;
	float dirY;

	glm::vec2 dir;

};

#endif // _BALL_INCLUDE

