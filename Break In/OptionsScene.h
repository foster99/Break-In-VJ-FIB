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
	~OptionsScene();
	
	// Inherited Methods
	void init();
	void update(int deltaTime);
	void render();

	// Menu Methods
	void changeTex();
	void setMode(Mode m);

private:
	TexturedQuad* texQuad;
	Texture texOptions;
	Texture texInstructions;
	Texture texCredits;
	Mode mode;
	//int actTexture; // REMOVE
};
#endif