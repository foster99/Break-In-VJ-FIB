#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include "Boss.h"

class Ball
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, float spdX, float spdY, bool freeze);
	bool update(int deltaTime, int& collided);
	void render(glm::mat4& displacement_mat);

	void moveWithPlayer(float spdX);

	void toogleMagnet();
	void toogleBossFight();
	void changeModifierX(float value);
	void changeModifierY(float value);
	
	void setTileMap(TileMap* tileMap);
	void setPlayer(Player* pla);
	void setPosition(const glm::vec2& pos);
	void setBoss(Boss* b);
	

	bool getMagnet();
	glm::vec2 getPosition();
	glm::ivec2 getPositionInTiles();
	glm::ivec2 getBasePositionInTiles();

	int getBallSize();
	glm::ivec2 getLastCollision();

private:
	glm::ivec2 tileMapDispl, lastCollision;
	glm::vec2 posBall;
	Texture tex;
	Sprite* sprite;
	TileMap* map;
	Player* player;
	Boss* boss;

	int sizeBall;
	float speed;
	float spdModifierX;
	float spdModifierY;
	bool magnet, bossFight;
};

#endif // _BALL_INCLUDE

