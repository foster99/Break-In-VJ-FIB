#ifndef _PASSWORDSCENE_INCLUDE
#define _PASSWORDSCENE_INCLUDE

#include <glm/glm.hpp>
#include "Scene.h"
#include "TexturedQuad.h"


// This class represents the menu scene

class PasswordScene : public Scene
{

public:
	PasswordScene();
	~PasswordScene();
	
	// Inherited Methods
	void init();
	void update(int deltaTime);
	void render();

	// Menu Methods
	void changeTex();

private:
	TexturedQuad* texQuad;
	Texture tex;
	//int actTexture; // REMOVE
};
#endif