#ifndef _MENUSCENE_INCLUDE
#define _MENUSCENE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480

// This class represents the menu scene

class MenuScene
{

public:
	MenuScene();
	~MenuScene();

	void init();
	void update(int deltaTime);
	void render();
	void changeTex();

private:
	void initShaders();

private:
	TexturedQuad* texQuad;
	ShaderProgram texProgram;
	Texture tex[2];
	int actTexture;
	float currentTime;
	glm::mat4 projection;

};
#endif