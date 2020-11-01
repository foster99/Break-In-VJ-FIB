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
	void update(int deltaTime);
	void render(glm::mat4& displacement_mat);
	
	glm::ivec2 getSize();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2& pos);
	void setTilesDisplacement(int t);
	void setRoom(int room);
	void toogleChangeBar();

	bool collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, float* posI, int speed, float &modifierY, float& modifierX);
	bool collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size, float* posJ, int speed);
	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, float* posJ, int speed);

	
private:
	bool movingX, movingY;
	int tiles_displacement, actRoom;
	glm::ivec2 tileMapDispl, sizePlayer, slideOffset;
	glm::vec2 posPlayer, displ_posPlayer;
	int jumpAngle, startYs, speedX, speedY, slideOffsetY, slideOffsetX;
	Texture spritesheet;
	Sprite *sprite;
	Slide *slide;
	TileMap *map;
};


#endif // _PLAYER_INCLUDE


