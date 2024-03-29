#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Slide.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	int update(int deltaTime);
	void render(glm::mat4& displacement_mat);
	
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2& pos);
	void setTilesDisplacement(int t);
	void setDeathAnimation(bool b);
	void setRoom(int room);
	void setBonus(int b);
	void setPosMainBall(glm::vec2 pos);
	
	glm::ivec2 getSize();
	glm::vec2 getPosition();
	glm::ivec2 getSlideOffset();
	float getSpeedX();
	int getBonus();
	int getCurrentRoom();
	bool getDeathAnimation();
	void updateEyesAnimation();
	void toogleChangeBar();

	bool collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, float* posI, int speed, float &modifierY, float& modifierX);
	bool collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size, float* posJ, int speed);
	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, float* posJ, int speed);

	static constexpr int still = 0;
	static constexpr int up = 1;
	static constexpr int down = 2;
	static constexpr int left = 3;
	static constexpr int right = 4;
	static constexpr int diag = 5;
	
private:
	bool movingX, movingY, deathAnimation;
	int tiles_displacement, currRoom, animation, bonus;
	glm::ivec2 tileMapDispl, sizePlayer, slideOffset;
	glm::vec2 posPlayer, displ_posPlayer;
	int jumpAngle, startYs, speedX, speedY, slideOffsetY, slideOffsetX;
	Texture spritesheet;
	Sprite *sprite;
	Slide *slide;
	glm::vec2 posMainBall;
	TileMap *map;
};


#endif // _PLAYER_INCLUDE


