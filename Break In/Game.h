#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Sound.h"

#include <GL/glew.h>
#include <GL/glut.h>
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

	void playBallSound();
	void playTitleSong();
	void stopTitleSong();

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
	void toogleChangeBar();

	// Application Scenes
	GameScene gameScene;				// Scene to renderDynamicTiles when playing
	StartMenuScene startMenuScene;		// Scene to renderDynamicTiles when showing the StartMenu
	OptionsScene optionsScene;			// Scene to renderDynamicTiles when showing the Options
	OptionsScene creditsScene;			// Scene to renderDynamicTiles when showing the Credits
	OptionsScene instructionsScene;		// Scene to renderDynamicTiles when showing the Instructions
	PasswordScene passwordScene;		// Scene to renderDynamicTiles when showing the Passwords Menu.

	
	stack<Mode> modeHist;				// Defines the mode history (startmenu, playing, credits, ...)

	bool keys[256], specialKeys[256];	// Store key states so that we can have access at any time
	
										//MenuScene scene;		// EXPERIMENT

	Sound ball_sound;
	Sound title_song;


};


#endif // _GAME_INCLUDE


