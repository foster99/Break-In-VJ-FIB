#ifndef _BONUS_INCLUDE
#define _BONUS_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class Bonus
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	bool update(int deltaTime);
	void render(glm::mat4& displacement_mat);

	// Getters
	int getActiveBonus();
	glm::vec2 getPosition();
	glm::ivec2 getPositionInTiles();
	
	// Setters
	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void setPlayer(Player* p);
	void setRoom(int r);
	void setBossMode(bool m);

	// Modifiers
	void changeModifierX(float value);
	void changeModifierY(float value);
	void restartTime();

	// Checkers
	bool isTimeToRender();
	bool checkCollision();

	// BONUS TYPES
	static constexpr int multipleBall	= 0;
	static constexpr int blaster		= 1;
	static constexpr int doubleSlide	= 2;
	static constexpr int magnet			= 3;
	static constexpr int doublePoints	= 4;
	static constexpr int wall			= 5;
	static constexpr int twix			= 6;

	static constexpr float bonusDelayTime		=  3500.f;
	static constexpr float bonusAliveTime		= 15000.f;
	static constexpr float bonusCollisionTime	=  1000.f;

private:
	glm::ivec2 tileMapDispl;
	Texture tex;
	Sprite* sprite;
	TileMap* map;
	Player* player;
	
	// Entity Props
	glm::vec2 posBonus;
	int room;
	int sizeBonus;
	int activeBonus;
	bool bossMode;

	// Time Control
	float bonusTime;
	float bonusTypeTime;
	bool timeToRender;

	// Speed Control
	float speed;
	int spdModifierX;
	int spdModifierY;
};

#endif // _BONUS_INCLUDE

