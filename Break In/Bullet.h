#ifndef _BULLET_INCLUDE
#define _BULLET_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Boss.h"

class Bullet
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	bool update(int deltaTime, int& collided);
	void render(glm::mat4& displacement_mat);

	void toogleBossFight();

	bool getDestroy();
	
	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void setDestroy();
	void setBoss(Boss* b);

	glm::ivec2 getLastCollision();

private:
	glm::ivec2 tileMapDispl, lastCollision,sizeBullet;;
	glm::vec2 posBullet;
	Texture tex;
	Sprite* sprite;
	TileMap* map;
	Boss* boss;
	float speed;
	bool destroy, bossFight;
};

#endif // _BULLET_INCLUDE