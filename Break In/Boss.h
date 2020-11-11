#ifndef _BOSS_INCLUDE
#define _BOSS_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include "ball.h"
#include <random>

class Boss
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	bool update(int deltaTime);
	void render(glm::mat4& displacement_mat);


	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void setPlayer(Player* p);
	void setBall(Ball* b);

	bool checkCollision();

	bool collisionWithPlayer();
	bool collisionWithBall();

private:
	glm::ivec2 tileMapDispl, bossSize;
	glm::vec2 posBoss, actualPlayerPos;
	Texture tex;
	Sprite* sprite;
	TileMap* map;
	Player* player;
	Ball* ball;

	// Speed Control
	float speed;
	int spdModifierX;
	int spdModifierY;
	bool movingRight;

	// Event Control
	int time2Switch, hits2Disable, currentHits;
	float elapsedTime;
};

#endif
