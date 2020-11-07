#ifndef _OPTIONSCENE_INCLUDE
#define _OPTIONSCENE_INCLUDE

#include "Scene.h"
#include "TexturedQuad.h"
#include <vector>

typedef unsigned short Mode;


class OptionsScene : public Scene
{

public:
	OptionsScene();
	OptionsScene(int n);
	~OptionsScene();
	
	// Inherited Methods
	void init();
	void update(int deltaTime);
	void render();

	void setNTextures(int n);
	void nextTexture();
	void prevTexture();
	int  getCurrTex();

	// Menu Methods
	void setTexture(int i, string filename);

private:

	TexturedQuad* texQuad;
	vector<Texture> tex;
	Mode mode;

	int currTexture;
};
#endif