#ifndef _MENUSCENE_INCLUDE
#define _MENUSCENE_INCLUDE

#include "Scene.h"
#include "TexturedQuad.h"
#include "Sprite.h"

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

	// Points Methods
	string intToStringOfNDigits(int i, int Ndigits);
	void updatePoints();
	void renderPoints();
	void setPoints(int p);
	void prepareCounters(vector<Sprite*>& quads, glm::vec3& coords, const glm::vec2& minCoords, ShaderProgram& program);


private:
	TexturedQuad* texQuad;
	Texture tex1, tex2;

	vector<Sprite*> pointsQuads;
	Texture digits;
	glm::vec3 pointsCoords;
	string points;

};
#endif