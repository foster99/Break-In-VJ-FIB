#ifndef _BOSS_INCLUDE
#define _BOSS_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
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
	void setFase(int f);
	void setStatus(int s);
	void setHunterMode(int hm);

	void nextAnimation();
	void trackPlayerPosition();
	bool arrivedTargetPos();


	void takeDamage(int hp, int source);
	
	bool onSlide(const glm::ivec2& pos, int sizeX, float& modifierY, float& modifierX);
	bool onSide(const glm::ivec2& pos, int sizeY);

	bool checkCollision();
	bool collisionWithPlayer();


	bool collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size, float* posJ, int speed);
	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, float* posJ, int speed);
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, float* posI, int speed, float& modifierY, float& modifierX);

	static constexpr int BALL = 0;
	static constexpr int BULLET = 1;

	static constexpr int NORMAL			= 0;
	static constexpr int STUNED			= 1;
	static constexpr int FIRE			= 2;
	static constexpr int SHIELD			= 3;
	static constexpr int BROKENSHIELD	= 4;
	
	static constexpr int SLEEP			= 0;
	static constexpr int TRACKING		= 1;
	static constexpr int MOVING			= 2;

	static constexpr float trackingTime = 1000.f;

private:
	glm::ivec2 tileMapDispl, bossSize;
	glm::vec2 posBoss, actualPlayerPos;
	Texture tex;
	Sprite* sprite;
	TileMap* map;
	Player* player;

	glm::vec2 targetPos;

	// Speed Control
	float speed;
	float spdModifierX;
	float spdModifierY;
	bool movingRight, hitted;

	// Event Control
	int fase;
	int bank;
	int time2Switch, hits2Disable, currentHits, status, healthPoints, shieldDurability, hunterMode;
	float elapsedTime, actTracking;
};

#endif
