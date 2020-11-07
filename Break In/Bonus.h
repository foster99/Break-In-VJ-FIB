#ifndef _BONUS_INCLUDE
#define _BONUS_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Bonus
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	bool update(int deltaTime);
	void render(glm::mat4& displacement_mat);

	void changeModifierX(float value);
	void changeModifierY(float value);
	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void restartTime();
	bool isTimeToRender();
	int getActiveBonus();
	glm::vec2 getPosition();
	glm::ivec2 getPositionInTiles();

	// BONUS TYPES
	static constexpr int multipleBall	= 0;
	static constexpr int blaster		= 1;
	static constexpr int doubleSlide	= 2;
	static constexpr int magnet			= 3;
	static constexpr int twix			= 4;

	static constexpr float bonusAliveTime = 15000.f;

private:
	glm::ivec2 tileMapDispl;
	Texture tex;
	Sprite* sprite;
	TileMap* map;
	
	// Entity Props
	glm::vec2 posBonus;
	int sizeBonus;
	int activeBonus;
	
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

