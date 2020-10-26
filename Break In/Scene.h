#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"

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

