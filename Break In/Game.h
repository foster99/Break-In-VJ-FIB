#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Sound.h"
#include <stack>
#include <iostream>

#include "GameScene.h"
#include "StartMenuScene.h"
#include "OptionsScene.h"
#include "PasswordScene.h"


typedef unsigned short Mode;
// Game is a singleton (a class with a single instance) that represents our whole application
// UwU

class Game
{

public:

	static constexpr int SCREEN_WIDTH	= 256;
	static constexpr int SCREEN_HEIGHT	= 192;
	static constexpr int TILESIZE		= 8;

	Game() {}
	
	static Game &instance()
	{
		static Game G;
		return G;
	}
	
	void init();
	void loadSounds();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

	// Different Modes
	static constexpr Mode exitGame = Mode(0);
	static constexpr Mode startMenu = Mode(1);
	static constexpr Mode playing = Mode(2);
	static constexpr Mode options = Mode(3);
	static constexpr Mode instructions = Mode(4);
	static constexpr Mode credits = Mode(5);
	static constexpr Mode password = Mode(6);

private:

	Mode currMode();
	void setMode(Mode newMode);
	void rollbackMode();
	void toggleGodMode();

	// Application Scenes
	GameScene gameScene;				// Scene to render when playing
	StartMenuScene startMenuScene;		// Scene to render when showing the StartMenu
	OptionsScene optionsScene;			// Scene to render when showing the Options
	PasswordScene passwordScene;		// Scene to render when showing the Passwords Menu.

	
	stack<Mode> modeHist;				// Defines the mode history (startmenu, playing, credits, ...)

	bool keys[256], specialKeys[256];	// Store key states so that we can have access at any time
	
										//MenuScene scene;		// EXPERIMENT
	//bool playing;						// CANVIAR PER HERENCIA? // REMOVE

	Sound bell_sound; // REMOVE


};


#endif // _GAME_INCLUDE


