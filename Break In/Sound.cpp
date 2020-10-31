#include "Sound.h"

Sound::Sound()
{
	engine = irrklang::createIrrKlangDevice();
}

Sound::Sound(string filename)
{
	engine = irrklang::createIrrKlangDevice();
	this->setFile(filename);
}

Sound::Sound(string filename, bool loopSound)
{
	engine = irrklang::createIrrKlangDevice();
	this->setFile(filename);
	this->setMode(loopSound);
	playing = false;
}

void Sound::setFile(string filename)
{
	this->path = "./sounds/" + filename;
}

void Sound::setMode(bool loopSound)
{
	this->loopSound = loopSound;
}

void Sound::play()
{
	if ((loopSound && !playing) || !loopSound) {
		//if (!engine) engine = irrklang::createIrrKlangDevice();
		engine->play2D(path.c_str(), loopSound);
		playing = true;
	}
}

void Sound::drop()
{
	if (playing) {
		engine->drop();
		playing = false;
	}
}
