#ifndef _OPTIONSCENE_INCLUDE
#define _OPTIONSCENE_INCLUDE

#include <glm/glm.hpp>
#include "Scene.h"
#include "TexturedQuad.h"

typedef unsigned short Mode;


class OptionsScene : public Scene
{

public:
	OptionsScene();
	OptionsScene(string filename);
	~OptionsScene();
	
	// Inherited Methods
	void init();
	void update(int deltaTime);
	void render();

	// Menu Methods
	void changeTex();
	void setTexture(string filename);

private:
	TexturedQuad* texQuad;
	Texture tex;
	Mode mode;
	//int actTexture; // REMOVE
};
#endif