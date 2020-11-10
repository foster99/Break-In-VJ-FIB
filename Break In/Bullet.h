#ifndef _BULLET_INCLUDE
#define _BULLET_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Bullet
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	bool update(int deltaTime);
	void render(glm::mat4& displacement_mat);

	bool getDestroy();
	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void setDestroy();

	glm::ivec2 getLastCollision();

private:
	glm::ivec2 tileMapDispl, lastCollision,sizeBullet;;
	glm::vec2 posBullet;
	Texture tex;
	Sprite* sprite;
	TileMap* map;
	float speed;
	bool destroy;
};

#endif // _BULLET_INCLUDE