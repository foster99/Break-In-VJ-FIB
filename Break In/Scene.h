#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"

class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

	static constexpr float SCREEN_X = 0;
	static constexpr float SCREEN_Y = 0;
	static constexpr float INIT_PLAYER_X_TILES = 11;
	static constexpr float INIT_PLAYER_Y_TILES = 19;
	static constexpr float CAMERA_WIDTH = 256;
	static constexpr float CAMERA_HEIGHT = 192;

private:
	void initShaders();

protected:
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};
#endif // _SCENE_INCLUDE

