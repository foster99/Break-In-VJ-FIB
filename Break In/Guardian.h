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
	void changeModifierX(float value);
	void changeModifierY(float value);
	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void setPlayer(Player* p);
	void restartTime();
	bool isAwake();
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
	glm::vec2 posGuardian;
	glm::ivec2 sizeGuardian;

	// Time Control
	int guardianMode;
	float guardianTime;

	// Speed Control
	float speed;
	int speedModifierX;
	int speedModifierY;
};
#endif // _GUARDIAN_INCLUDE

