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

	void setShield1(bool s);
	void setShield2(bool s);


	bool isAlive();
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


private:
	glm::ivec2 tileMapDispl, bossSize;
	glm::vec2 posBoss, actualPlayerPos;
	Texture tex;
	Sprite* sprite;
	TileMap* map;
	Player* player;

	static constexpr int waiting = 0;
	static constexpr int part1	= 1;
	static constexpr int part2	= 2;
	static constexpr int part3	= 3;
	static constexpr int part4	= 4;
	static constexpr int part5	= 5;
	static constexpr int part6	= 6;
	static constexpr int done	= 7;

	// Fase Control
	int fase1_status;

	// Speed Control
	float speed;
	int spdModifierX;
	int spdModifierY;
	bool movingRight, hitted;

	// Event Control
	bool alive;
	int fase;
	int bank;

	bool shield1, shield2;

	int time2Switch, hits2Disable, currentHits, status, healthPoints, shieldDurability;
	float elapsedTime;
	float brickTime;
	float statusTime;
};

#endif
