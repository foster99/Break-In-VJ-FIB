#ifndef _BANK_INCLUDE
#define _BANK_INCLUDE


//#include "LevelScene.h"
//#include "TransitionScene.h"
//#include 


// Level is a class that represent a Level of the Game.

class Bank
{

public:

	Bank() {

	}

	void loadBank();
	bool update(int deltaTime);
	void render();


private:

	int id;                       // Continue to play game?

	//vector<LevelScene> scenes;
	//TransitionScene trans_scene;


};


#endif // _BANK_INCLUDE

