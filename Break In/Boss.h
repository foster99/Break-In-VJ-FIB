#ifndef _BOSS_INCLUDE
#define _BOSS_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class Boss
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	bool update(int deltaTime);
	void render(glm::mat4& displacement_mat);


	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void setPlayer(Player* p);

	bool checkCollision();

private:
	glm::ivec2 tileMapDispl, bossSize;
	glm::vec2 posBoss;
	Texture tex;
	Sprite* sprite;
	TileMap* map;
	Player* player;

	// Speed Control
	float speed;
	int spdModifierX;
	int spdModifierY;
	bool movingRight;
};

#endif
