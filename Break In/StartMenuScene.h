#ifndef _MENUSCENE_INCLUDE
#define _MENUSCENE_INCLUDE

#include "Scene.h"
#include "TexturedQuad.h"


// This class represents the menu scene

class StartMenuScene : public Scene
{

public:
	StartMenuScene();
	~StartMenuScene();
	
	// Inherited Methods
	void init();
	void update(int deltaTime);
	void render();

	// Menu Methods
	void changeTex();

private:
	TexturedQuad* texQuad;
	Texture tex1, tex2;
	//int actTexture; // REMOVE
};
#endif