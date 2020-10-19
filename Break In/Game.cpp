#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	playing = false;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	loadSounds();

	Mscene.init();
	scene.init();
}

void Game::loadSounds()
{
	bell_sound = Sound("bell.wav", false);
}

bool Game::update(int deltaTime)
{
	if(!playing)
		Mscene.update(deltaTime);
	else
		scene.update(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(!playing)
		Mscene.render();
	else
		scene.render();
}

void Game::keyPressed(int key)
{
	switch (key) {
		case 27: // Escape code
			bPlay = false;
			break;
		case 'c':
			if (!playing) Mscene.changeTex();
			break;
		case ' ':
			playing = !playing;
			break;
		case 's':
			bell_sound.play();
			break;

		default:
			break;
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





