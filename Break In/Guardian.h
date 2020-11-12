#ifndef _GUARDIAN_INCLUDE
#define _GUARDIAN_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class Guardian
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	bool update(int deltaTime);
	void render(glm::mat4& displacement_mat);

	void alarmOn();
	void nextAnimation();
	bool arrivedTargetPos();
	void trackPlayerPosition();
	void changeModifierX(float value);
	void changeModifierY(float value);
	int getRoom();
	void setRoom(int r);
	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void setPlayer(Player* p);
	void restartTime();
	void toogleBossFight();
	bool isAwake();
	bool checkCollision();
	glm::vec2 getPosition();
	glm::ivec2 getPositionInTiles();

	// Guadian Modes
	static constexpr int sleep		= 0;
	static constexpr int tracking	= 1;
	static constexpr int moving		= 2;

	static constexpr float trackingTime = 5000.f;

private:
	glm::ivec2 tileMapDispl;
	Texture tex;
	Sprite* sprite;
	TileMap* map;
	Player* player;
	int animation, count, dir;
	
	// Entity Props
	int guardianRoom;
	glm::vec2 posGuardian;
	glm::ivec2 sizeGuardian;

	// Time Control
	int guardianMode;
	float guardianTime;
	bool bossFight;

	// Speed Control
	glm::vec2 targetPos;
	float speed;
	float speedModifierX;
	float speedModifierY;
};
#endif // _GUARDIAN_INCLUDE

