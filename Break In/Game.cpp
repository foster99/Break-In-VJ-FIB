#include "Game.h"

void Game::init()
{
	modeHist.push(startMenu);

	//playing = false; // REMOVE
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Sound Initialization
	// Should sound go into Scene clases ????? REMOVE
	loadSounds();

	// Scene initialization
	startMenuScene.init();
	gameScene.init();
	passwordScene.init();
	optionsScene.init(); 
	creditsScene.init(); 
	instructionsScene.init(); 

	// Set options scene textures
	creditsScene.setTexture("images/credits_screen.png");
	instructionsScene.setTexture("images/bank_items.png");
	optionsScene.setTexture("images/options_screen.png");
}

void Game::loadSounds()
{
	//ball_sound = Sound("tueeeeeeeeee.wav", false);
	ball_sound = Sound("ball.wav", false);
	title_song = Sound("title_song.wav", true);
}

bool Game::update(int deltaTime)
{
	//if(!playing)
	//	Mscene.update(deltaTime);
	//else

	switch (currMode()) {
	case startMenu:
		startMenuScene.update(deltaTime);
		playTitleSong();
		break;

	case playing:
		gameScene.update(deltaTime);
		stopTitleSong();
		break;

	case instructions:
		instructionsScene.update(deltaTime);
		break;

	case credits:
		creditsScene.update(deltaTime);
		break;

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

void Game::render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (currMode()) {
	case startMenu:
		startMenuScene.render();
		break;
	
	case playing:
		gameScene.render();
		break;

	case instructions:
		instructionsScene.render();
		break;

	case credits:
		creditsScene.render();
		break;

	case options:
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
			case 'b':	toogleChangeBar();	break;
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

	case credits:
		switch (key) {
			case ESC:	rollbackMode();	break;
			default:	break;
		} break;

	case password:
		switch (key) {
			case ESC:	rollbackMode();	break;
			default:	break;
		} break;
	}

		
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

void Game::playBallSound()
{
	ball_sound.play();
}

void Game::playTitleSong()
{
	title_song.play();
}

void Game::stopTitleSong()
{
	title_song.drop();
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

void Game::toogleChangeBar()
{
	gameScene.toogleChangeBar();
}




