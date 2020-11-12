#include "Game.h"

Game::Game() {}

void Game::init()
{
	modeHist.push(startMenu);
	
	//playing = false; // REMOVE
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Loads
	loadSounds();
	loadPoints();

	// Scenes initialization
	startMenuScene.init();
	passwordScene.init();
	optionsScene.init(); 
	creditsScene.init(); 
	instructionsScene.init(); 

	// GameScene initialization
	bank = 1;
	restartGameScene();

	// OptionsScene textures to select next screen
	optionsScene.setNTextures(3);
	optionsScene.setTexture(0, "images/options_screen_credits.png");
	optionsScene.setTexture(1, "images/options_screen_instructions.png");
	optionsScene.setTexture(2, "images/options_screen_exit.png");

	// Set options scene textures
	creditsScene.setTexture(0, "images/credits_screen.png");
	instructionsScene.setTexture(0, "images/instructions_screen.png");
}

void Game::loadSounds()
{
	title_song			= Sound("title_song.wav", true);
	alarm_sound			= Sound("alarm.wav", true);
	gameover_song		= Sound("gameover.wav", true);
	win_song			= Sound("win_song.wav", true);
	boss_song			= Sound("boss_song.wav", true);

	slide_sound			= Sound("player_collision.wav", false);
	brick_sound			= Sound("brick_collision.wav", false);
	doubleSlide_sound	= Sound("double_slide.wav", false);
	bonus_sound			= Sound("bonus.wav", false);
	loseLife_sound		= Sound("lose_live.wav", false);
	blast_sound			= Sound("blast.wav", false);
	money_sound_01		= Sound("moneyBag_01.wav", false);
	money_sound_02		= Sound("moneyBag_02.wav", false);
	money_sound_03		= Sound("moneyBag_03.wav", false);
	greenCard_sound		= Sound("greenCard.wav", false);
	bossHit_sound		= Sound("hit.wav", false);
	boss_animation_sound= Sound("boss_animation_sound.wav", false);
	shield_sound		= Sound("shield.wav", false);
	bomb_sound			= Sound("bomb.wav", false);
}

void Game::loadPoints()
{

	ifstream fin;
	string line;
	stringstream sstream;
	int p;

	fin.open("levels/maxpoints.txt");
	if (!fin.is_open())
		return;
	
	getline(fin, line);
	sstream.str(line);
	sstream >> p;
	getline(fin, line);

	setMaxPoints(p);
}

void Game::restartGameScene()
{
	gameScene.init();
	gameScene.setBank(bank);
	gameScene.setRoom(1);
	gameScene.setMoney(0);
	gameScene.setPoints(0);
	gameScene.setLives(5);
	gameScene.startBank();
	setMode(startMenu);
}

bool Game::update(int deltaTime)
{
	switch (currMode()) {
	case startMenu:
		startMenuScene.setPoints(max_points);
		startMenuScene.update(deltaTime);
		playTitleSong();
		stopAlarmSound();
		stopBossSong();
		break;

	case playing:
		if (gameScene.getWin()) {						// WIN

			// run animation salu2
			if (bank == 3)	bank = 1;
			else			bank++;
			
			gameScene.setBank(bank);
			gameScene.setPoints(0);
			gameScene.startBank();
			gameScene.setWin(false);
		}
		if (gameScene.getGoBoss()) {						// WIN

			// No hace falta cambiar el banco porque el numero del banco
			// es el mismo numero que el del boss.
			gameScene.startBoss();
			gameScene.setGoBoss(false);
		}
		else if (gameScene.getGameOver()) {				// GAMEOVER
			
			if (gameScene.getPoints() > max_points)
				setMaxPoints(gameScene.getPoints());

			restartGameScene();
		}
		else  {											// CONTINUE
			gameScene.update(deltaTime);
		}

		stopTitleSong();
		break;

	case instructions:
		instructionsScene.update(deltaTime);
		stopAlarmSound();
		break;

	case credits:
		creditsScene.update(deltaTime);
		stopAlarmSound();
		break;

	case options:
		optionsScene.update(deltaTime);
		stopAlarmSound();
		break;

	case password:
		passwordScene.update(deltaTime);
		stopBossSong();
		stopAlarmSound();
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
	static constexpr int ENTER = 13;

	// Para captar las teclas mayusculas como minusculas;
	int lower_key = key;
	if ('A' <= key && key <= 'Z') lower_key -= 'A';

	switch (currMode()) {
	case startMenu:
		cout << "start" << endl;
		switch (lower_key) {
			case ESC:	setMode(options);	break;
			case 'p':	setMode(password);	break;
			case 'e':	setMode(exitGame);	break;
			case ' ':	setMode(playing);	break;
			default:						break;
		} break;

	case playing:
		switch (lower_key) {
			case ESC:	setMode(options);		break;
			case 'G':
			case 'g':	toggleGodMode();		break;
			case 'k':
				if (!gameScene.inGodMode()) break;
				gameScene.setWin(true);
			case 'x':
				if (!gameScene.inGodMode()) break;
				gameScene.insertBrick(0, 0);
				break;
			case '+':	
				if (!gameScene.inGodMode()) break;
				if (keys['b']) gameScene.createNewBall(1.f, 1.f);
				if (keys['r']) gameScene.nextRoom();
				if (keys['d']) gameScene.openDoor();
				if (keys['l']) gameScene.addLive();
				// if (keys['p']) /* NEXT BONUS */
				break;
			case '-':
				if (!gameScene.inGodMode()) break;
				if (keys['b']) gameScene.deleteLastBall();
				if (keys['r']) gameScene.prevRoom();
				if (keys['d']) gameScene.closeDoor();
				if (keys['l']) gameScene.subLive();
				// if (keys['p']) /* PREVIOUS BONUS */
				break;
			
			//case '0':
			case '1':
			case '2':
			case '3':
			//case '4':
			//case '5':
			//case '6':
			//case '7':
			//case '8':
			//case '9':
				if (!gameScene.inGodMode()) break;
				gameScene.setBank(lower_key - '0');
				gameScene.startBank();
				break;
			case ' ':
				if (gameScene.createNewBullets())
					playBlastSound();
				break;
			default:	break;
		} break;

	case options:
		switch (lower_key) {
			case ESC:	rollbackMode();			break;
			case ENTER:	
				switch (optionsScene.getCurrTex())
				{
				case 0:	setMode(credits);		break;
				case 1:	setMode(instructions);	break;
				case 2:	setMode(startMenu);		break;
				default:						break;
				}
				playPlayerSound();
				break;
			default:	break;
		} break;

	case instructions: 
		switch (lower_key) {
			case ESC:	rollbackMode();	break;
			default:	break;
		} break;

	case credits:
		switch (lower_key) {
			case ESC:	rollbackMode();	break;
			default:	break;
		} break;

	case password:
		switch (lower_key) {
			case ESC:	rollbackMode();	break;
			//case '0':
			case '1':
			case '2':
			case '3':
				//case '4':
				//case '5':
				//case '6':
				//case '7':
				//case '8':
				//case '9':
				bank = lower_key - '0';
				rollbackMode();
				restartGameScene();
				setMode(playing);
				break;
			default:	break;
		} break;
	}

	keys[key] = true;
	keys[lower_key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	switch (currMode()) {
	case startMenu:
		switch (key) {
		default:	break;
		} break;

	case playing:
		switch (key) {
		default:	break;
		} break;

	case options:
		switch (key) {
		case GLUT_KEY_UP:
			playBrickSound();
			optionsScene.prevTexture();
			break;
		case GLUT_KEY_DOWN:
			playBrickSound();
			optionsScene.nextTexture();
			break;
		default:	break;
		} break;

	case instructions:
		switch (key) {
		default:	break;
		} break;

	case credits:
		switch (key) {
		default:	break;
		} break;

	case password:
		switch (key) {
		default:	break;
		} break;
	}
	
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

void Game::setMaxPoints(int p)
{
	if (p > max_points) max_points = p;
	startMenuScene.setPoints(max_points);
}

void Game::playBallSound()
{
	
}

void Game::playBrickSound()
{
	brick_sound.play();
}

void Game::playBonusSound()
{
	bonus_sound.play();
}

void Game::playPlayerSound()
{
	slide_sound.play();
}

void Game::playDoubleSlideSound()
{
	doubleSlide_sound.play();
}

void Game::playLoseLiveSound()
{
	loseLife_sound.play();
}

void Game::playBlastSound()
{
	blast_sound.play();
}

void Game::playGreenCardSound()
{
	greenCard_sound.play();
}

void Game::playMoneySound()
{
	std::default_random_engine generator;
	std::uniform_int_distribution<int> rand(0, 2);

	switch (rand(generator)) {
	case 0: money_sound_01.play(); break;
	case 1: money_sound_02.play(); break;
	case 2: money_sound_03.play(); break;
	}
}

void Game::playBossAnimationSound()
{
	boss_animation_sound.play();
}

void Game::playBossHitSound()
{
	bossHit_sound.play();
}

void Game::playBombSound()
{
	bomb_sound.play();
}

void Game::playShieldSound()
{
	shield_sound.play();
}

void Game::playAlarmSound()
{
	alarm_sound.play();
}

void Game::playTitleSong()
{
	title_song.play();
}

void Game::stopTitleSong()
{
	title_song.drop();
}

void Game::playGameOverSong()
{
	gameover_song.play();
}

void Game::playWinSong()
{
	win_song.play();
}

void Game::playBossSong()
{
	boss_song.play();
}

void Game::stopGameOverSong()
{
	gameover_song.drop();
}

void Game::stopWinSong()
{
	win_song.drop();
}

void Game::stopBossSong()
{
	boss_song.drop();
}

void Game::stopAllSongs()
{
	stopAlarmSound();
	stopTitleSong();
	stopGameOverSong();
	stopWinSong();
	stopBossSong();
}

void Game::stopAlarmSound()
{
	alarm_sound.drop();
}

Mode Game::currMode()
{
	return modeHist.top();
}

void Game::setMode(Mode newMode)
{
	if (currMode() == startMenu && newMode == playing)
		restartGameScene();
	
	if (newMode == startMenu)
		modeHist = {};

	modeHist.push(newMode);
}

void Game::rollbackMode()
{
	playBonusSound();
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




