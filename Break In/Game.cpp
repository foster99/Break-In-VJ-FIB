#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "StartMenuScene.h"

void Game::init()
{
	modeHist.push(startMenu);

	//playing = false; // REMOVE
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	// Sound Initialization
	// Should sound go into Scene clases ????? REMOVE
	loadSounds();

	// Scene initialization
	startMenuScene.init();
	gameScene.init();
	optionsScene.init();
	passwordScene.init();
}

void Game::loadSounds()
{
	bell_sound = Sound("bell.wav", false); // REMOVE
}

bool Game::update(int deltaTime)
{
	//if(!playing)
	//	Mscene.update(deltaTime);
	//else

	switch (currMode()) {
	case startMenu:
		startMenuScene.update(deltaTime);
		break;

	case playing:
		gameScene.update(deltaTime);
		break;

	case instructions:
	case credits:
	case options:
		optionsScene.update(deltaTime);
		break;

	case password:
		passwordScene.update(deltaTime);
		break;

	case exitGame: break;
	}
	
	return currMode() != exitGame;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (currMode()) {
	case startMenu:
		startMenuScene.render();
		break;

	case playing:
		gameScene.render();
		break;

	case instructions:
	case credits:
	case options:
		optionsScene.setMode(currMode());
		optionsScene.render();
		break;

	case password:
		passwordScene.render();
		break;

	case exitGame: break;
	}
}

void Game::keyPressed(int key)
{
	static constexpr int ESC = 27;

	switch (currMode()) {
	case startMenu:
		cout << "start" << endl;
		switch (key) {
			case ESC:	setMode(options);	break;
			case 'p':	setMode(password);	break;
			case 'e':	setMode(exitGame);	break;
			case ' ':	setMode(playing);	break;
			default:	break;
		} break;

	case playing:
		switch (key) {
			case ESC:	setMode(options);	break;
			case 'g':	toggleGodMode();	break;
			default:	break;
		} break;

	case options:
		switch (key) {
			case ESC:	rollbackMode();			break;
			case 'i':	setMode(instructions);	break;
			case 'c':	setMode(credits);		break;
			case 'e':	setMode(startMenu);		break;
			default:	break;
		} break;

	case instructions: 
		switch (key) {
			case ESC:	rollbackMode();	break;
			default:	break;
		} break;

	case credits: break;
		switch (key) {
			case ESC:	rollbackMode();	break;
			default:	break;
		} break;

	case password: break;
		switch (key) {
			case ESC:	rollbackMode();	break;
			default:	break;
		} break;
	}

	//
	//switch (key) {
	//	case 'm': // Options Key
	//		if (!playing) menuScene.changeTex(); // REMOVE
	//		break;

	//	case 's': // REMOVE
	//		bell_sound.play();
	//		break;

	//	default:
	//		break;
	//}
		
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

Mode Game::currMode()
{
	return modeHist.top();
}

void Game::setMode(Mode newMode)
{
	if (newMode == startMenu)
		modeHist = {};

	modeHist.push(newMode);
}

void Game::rollbackMode()
{
	if (!modeHist.empty()) modeHist.pop();
}

void Game::toggleGodMode()
{
	gameScene.toggleGodMode();
}





