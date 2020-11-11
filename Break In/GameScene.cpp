#include "Game.h"
#include "GameScene.h"
#include "MenuTileMap.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

GameScene::GameScene()
{
	player = NULL;
}

GameScene::~GameScene()
{
	this->Scene::~Scene();

	if (player   != NULL)	delete player;
	if (ball     != NULL)	delete ball;
	if (guardian != NULL)	delete guardian;
	if (bonus    != NULL)	delete bonus;
}

void GameScene::init() {

	this->Scene::init();

	// Initializations
	room = 1;
	room_old = 1;
	lives = 3;
	scrolling = false;
	bank = 1;
	tiles_displacement = -48;
	stride = 0;
	end = 0;
	godMode = false;
	restarted = false;
	alive = true;
	gameOver = false;
	gameOverAnimation = starts;
	win = false;
	winAnimation = starts;
	timeToDelete = 0;
	auxTime = 0.f;

	// TESTIGO GOD MODE
	godModeTex.loadFromFile("images/godmode.png", TEXTURE_PIXEL_FORMAT_RGBA);
	godModeSprite = Sprite::createSprite(glm::ivec2(60,8), glm::vec2(1.f, 1.f), &godModeTex, &texProgram);
	godModeSprite->setNumberAnimations(1);
	godModeSprite->addKeyframe(0, glm::vec2(0.f));
	godModeSprite->changeAnimation(0);
	godModeSprite->setPosition(glm::vec2(24.f * 8.f + 3.f, 9.f * 8.f - 2.f));
}

void GameScene::update(int deltaTime) {

	this->Scene::update(deltaTime);

	glm::vec2 previousPlayerPos = player->getPosition();

	if ((timeToDelete % 4) == 0)					// DELETEAR BULLETS SI DESTROY TRUE
	{
		for (auto it = bullets.begin(); it != bullets.end();)
			if ((*it)->getDestroy())	it = bullets.erase(it);
			else						++it;

		timeToDelete = 0;
	}
	if (!alive)										// ESTAMOS MUERTOS
	{									
		auxTime += deltaTime;
		if (auxTime > 175.f) {
			auxTime = 0;
			player->update(deltaTime);
		}
		alive = !(player->getDeathAnimation());

		if (alive && bossIsAlive) restartPlayerBallBoss();
		if (alive)				  restartPlayerBallBank();
		else       return;
	}
	else if (gameOver)								// GAME OVER, PERO HAY QUE MOSTRAR LA ANIMACION
	{							
		auxTime += deltaTime;
		animateGameOver();
		return;
	}
	else if (win)									// GAME OVER, PERO HAY QUE MOSTRAR LA ANIMACION
	{							
		auxTime += deltaTime;
		animateWin();
		return;
	}
	else if (goBoss)								// GAME OVER, PERO HAY QUE MOSTRAR LA ANIMACION
	{
		auxTime += deltaTime;
		animateGoBoss();
		return;
	}
	else if (lastBallisDead())						// COMPROBAR SI HEMOS MUERTO 
	{
		playerLosesLife();
		player->update(deltaTime);
	}
	else if (scrolling)								// ESTAMOS REALIZANDO SCROLL ENTRE ROOMS
	{
		tiles_displacement += stride;
		scrolling = tiles_displacement != end;
		bonus->restartTime();
	}
	else if (changeOfRoom())						// COMPROBAR SI HAY QUE CAMBIAR DE ROOM
	{
		if (room_old < room)			stride = +1;
		else /*(room_old > currRoom)*/	stride = -1;

		switch (room) {
		case 1: end = -48;	break;
		case 2: end = -24;	break;
		case 3: end = 0;	break;
		}

		bonus->setRoom(room);
		bonus->restartTime();
	}
	else if (greenCardAnimation == starts){
		if (convertPointsToMoney())
			greenCardAnimation = finished;
		
		return;
	}
	else {
		int collided;
		for (Ball* ball : balls) {
			if (!ball->getMagnet() && ball->update(deltaTime,collided))
			{
				glm::ivec2 tile = ball->getLastCollision();
				
				if (collided == PLAYER_COLLISION) {
					Game::instance().playPlayerSound(); //  IMPACTO JODADORE
					if (player->getBonus() == Bonus::magnet || player->getBonus() == Bonus::twix)
						ball->toogleMagnet();
				}
				else {
					switch (map->ballTileCollision(tile[0], tile[1]))
					{
					case Tile::moneyBag:
						Game::instance().playMoneySound();
						money += 100;
						break;

					case Tile::coin:
						Game::instance().playBrickSound();
						money += 50;
						break;

					case Tile::blueSpheres:
						Game::instance().playMoneySound();
						money += 250;
						break;

					case Tile::brickBlue:
					case Tile::brickRed:
					case Tile::brickYellow:
					case Tile::brickGreen:
					case Tile::brickLow:
						Game::instance().playBrickSound();
						points += 100;
						break;

					case Tile::outCard:
						takeGreenCard();
						break;

					case Tile::alarm:
						guardian->alarmOn();
						break;

					default: break;
					}
				}

				menuMap->setMoney(money);
				menuMap->setPoints(points);

			}
		}
		
		for (Bullet* bullet : bullets) {
			if (!bullet->getDestroy() && bullet->update(deltaTime,collided)) {
				glm::ivec2 tile = bullet->getLastCollision();

				switch (map->bulletTileCollision(tile[0], tile[1]))
				{

				case Tile::brickBlue:
				case Tile::brickRed:
				case Tile::brickYellow:
				case Tile::brickGreen:
				case Tile::brickLow:
					Game::instance().playBrickSound();
					points += 100;
					break;
				default: break;
				}
				bullet->setDestroy();
				menuMap->setMoney(money);
				menuMap->setPoints(points);
			}
		}

		map->setRoom(room);
		menuMap->setLives(lives);
		player->setRoom(room);
		player->setTilesDisplacement(tiles_displacement);
		player->setPosMainBall(balls.front()->getPosition());
	}

	if (bonus->update(deltaTime)) {

		switch (bonus->getActiveBonus())
		{
		case Bonus::multipleBall:
			createNewBall(-0.8, 1);
			createNewBall(0.8, 1);
			break;

		case Bonus::doublePoints:
			break;

		case Bonus::none:
			break;

		case Bonus::twix:
		case Bonus::blaster:
		case Bonus::doubleSlide:
		case Bonus::magnet:
			player->setBonus(bonus->getActiveBonus());
			break;
		}
		
		Game::instance().playBonusSound();
	}
	if (guardian->getRoom() == player->getCurrentRoom()) {
		if (guardian->update(deltaTime))
			if (!godMode) playerLosesLife();
	}
	else {
		map->setAlarm(false);
		Game::instance().stopAlarmSound();
		guardian->restartTime();
	}

	if (player->getBonus() > 0) {
		switch (player->getBonus()) {
		case Bonus::multipleBall:	menuMap->setLine("AII LMAO" , "4POGGERS"); break;
		case Bonus::blaster:		menuMap->setLine(" BLASTER" , "  POWER "); break;
		case Bonus::doubleSlide:	menuMap->setLine(" DOUBLE " , "  SLIDE "); break;
		case Bonus::magnet:			menuMap->setLine("  MAGNET" , " 4THEWIN"); break;
		case Bonus::twix:			menuMap->setLine("  SNOOP " , "DOGGYDOG"); break;
		default: break;
		}
	}
	else menuMap->setLine(" SINGLE ", "  SLIDE ");
	map->prepareDynamicArrays();
	displacement_mat = glm::translate(glm::mat4(1.f), glm::vec3(0.f, float(tiles_displacement * 8), 0.f));

	if(checkBallSlide())
		ballOnSlide += deltaTime;
	
	// Pelotas Magnetizadas

	bool reset				= false;
	int lastMov				= player->update(deltaTime);
	glm::vec2 newPosistion	= player->getPosition();
	bool movedY				= (!restarted && previousPlayerPos.y != newPosistion.y);

	for (Ball* ball : balls) {
		if (ball->getMagnet()) {
			if (movedY) { // CAMVIAR: mirar pos.y abans de update i si despres ha canviat a la verga sa pilota
				reset = true;
				ball->toogleMagnet();
			}
			else if (lastMov == Player::right || lastMov == Player::left)
				ball->moveWithPlayer(player->getSpeedX());
			else if (ballOnSlide >= 3000) {
				reset = true;
				ball->toogleMagnet();
			}
		}
	}

	// BOSS
	if (bossIsAlive) {
		if (boss->update(deltaTime))
			if (!godMode) playerLosesLife();
	}
	
	if (reset)
		ballOnSlide = 0;
	
	++timeToDelete;
	
	if (goBossAnimation == starts)
		setGoBoss(!map->moneyLeft());

	if (goBossAnimation == finished)
		setWin(!bossIsAlive);

	restarted = false;
}

void GameScene::render()
{	
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", displacement_mat);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	
	map->render();
	if (!scrolling)		player->render(displacement_mat);
	
	bonus->render(displacement_mat);
	guardian->render(displacement_mat);
	
	for (Ball* ball : balls) {
		ball->render(displacement_mat);
	}

	for (Bullet* bullet : bullets) {
		bullet->render(displacement_mat);
	}
	
	if (bossIsAlive)
		boss->render(displacement_mat);

	// Render Lateral Menu
	glm::mat4 menu_modelview = glm::translate(glm::mat4(1.f), glm::vec3(192.f, 0.f, 0.f));
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", menu_modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	menuMap->render();

	if (godMode) godModeSprite->render(glm::mat4(1));


	if (greenCardAnimation == starts) {
		greenCardSprite->render(glm::mat4(1));
	}

	if (winAnimation >= 0)
	{
		winSprite->changeAnimation(0);
		winSprite->render(glm::mat4(1));

		antonioSprite->changeAnimation(antonioAnimation);
		antonioSprite->render(glm::mat4(1));
	}

	if (gameOverAnimation >= 0)
	{
		gameOverSprite->changeAnimation(gameOverAnimation);
		gameOverSprite->render(glm::mat4(1));

		antonioSprite->changeAnimation(antonioAnimation);
		antonioSprite->render(glm::mat4(1));
	}

	if (goBoss && goBossAnimation == starts)
	{
		goBossSprite->changeAnimation(0);
		goBossSprite->render(glm::mat4(1));
	}
}

void GameScene::setUpGameOverSprite()
{
	string path = "images/gameover_trans.png";
	gameOverAnimation = starts;
	antonioAnimation = 0;

	// SPRITE AND TEXTURE SET-UP
	gameOverTex.loadFromFile(path, TEXTURE_PIXEL_FORMAT_RGBA);
	gameOverSprite = Sprite::createSprite(glm::ivec2(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT), glm::vec2(1.f / 4.f, 1.f), &gameOverTex, &texProgram);
	gameOverSprite->setNumberAnimations(4);

	gameOverSprite->addKeyframe(0, glm::vec2(0.f / 4.f, 0.f));
	gameOverSprite->addKeyframe(1, glm::vec2(1.f / 4.f, 0.f));
	gameOverSprite->addKeyframe(2, glm::vec2(2.f / 4.f, 0.f));
	gameOverSprite->addKeyframe(3, glm::vec2(3.f / 4.f, 0.f));

	gameOverSprite->changeAnimation(0);
	gameOverSprite->setPosition(glm::vec2(0.f));

	// SPRITE AND TEXTURE SET-UP
	antonioTex.loadFromFile("images/antonio.png", TEXTURE_PIXEL_FORMAT_RGBA);
	antonioSprite = Sprite::createSprite(glm::ivec2(25, 30), glm::vec2(1.f / 4.f, 1.f), &antonioTex, &texProgram);
	antonioSprite->setNumberAnimations(4);

	antonioSprite->addKeyframe(0, glm::vec2(0.f / 4.f, 0.f));
	antonioSprite->addKeyframe(1, glm::vec2(1.f / 4.f, 0.f));
	antonioSprite->addKeyframe(2, glm::vec2(2.f / 4.f, 0.f));
	antonioSprite->addKeyframe(3, glm::vec2(3.f / 4.f, 0.f));
	antonioSprite->changeAnimation(0);

	antonioPos = glm::vec2(10.f, 192.f - 30.f);
	antonioSprite->setPosition(antonioPos);

	auxTime = 0.f;
}

void GameScene::setUpWinSprite()
{
	string path;
	if (bank < 10)	path = "images/bank_win_0" + to_string(bank) + ".png";
	else			path = "images/bank_win_"  + to_string(bank) + ".png";
	
	winAnimation = starts;

	// SPRITE AND TEXTURE SET-UP
	winTex.loadFromFile(path, TEXTURE_PIXEL_FORMAT_RGBA);
	winSprite = Sprite::createSprite(glm::ivec2(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT), glm::vec2(1.f, 1.f), &winTex, &texProgram);
	winSprite->setNumberAnimations(1);
	winSprite->addKeyframe(0, glm::vec2(0.f));
	winSprite->changeAnimation(0);
	winSprite->setPosition(glm::vec2(0.f));

	// SPRITE AND TEXTURE SET-UP
	antonioTex.loadFromFile("images/antonio.png", TEXTURE_PIXEL_FORMAT_RGBA);
	antonioSprite = Sprite::createSprite(glm::ivec2(25, 30), glm::vec2(1.f / 4.f, 1.f), &antonioTex, &texProgram);
	antonioSprite->setNumberAnimations(4);

	antonioSprite->addKeyframe(0, glm::vec2(0.f / 4.f, 0.f));
	antonioSprite->addKeyframe(1, glm::vec2(1.f / 4.f, 0.f));
	antonioSprite->addKeyframe(2, glm::vec2(2.f / 4.f, 0.f));
	antonioSprite->addKeyframe(3, glm::vec2(3.f / 4.f, 0.f));
	antonioSprite->changeAnimation(0);

	antonioPos = glm::vec2(45.f, 192.f - 60.f);
	antonioSprite->setPosition(antonioPos);

	auxTime = 0.f;
}

void GameScene::setUpGreenCardSprite()
{
	// SPRITE AND TEXTURE SET-UP
	greenCardTex.loadFromFile("images/greenCard.png", TEXTURE_PIXEL_FORMAT_RGBA);
	greenCardSprite = Sprite::createSprite(glm::ivec2(19.f*8.f,14.f * 8.f ), glm::vec2(1.f, 1.f), &greenCardTex, &texProgram);
	greenCardSprite->setNumberAnimations(1);
	greenCardSprite->addKeyframe(0, glm::vec2(0.f));
	greenCardSprite->changeAnimation(0);
	greenCardSprite->setPosition(glm::vec2(2.5f * 8.f, 5.f * 8.f));

}

void GameScene::setUpGoBossSprite()
{
	// SPRITE AND TEXTURE SET-UP
	goBossTex.loadFromFile("images/boss_animation.png", TEXTURE_PIXEL_FORMAT_RGBA);
	goBossSprite = Sprite::createSprite(glm::ivec2(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT), glm::vec2(1.f, 1.f), &goBossTex, &texProgram);
	goBossSprite->setNumberAnimations(1);
	goBossSprite->addKeyframe(0, glm::vec2(0.f));
	goBossSprite->changeAnimation(0);
	goBossSprite->setPosition(glm::vec2(0));

	Game::instance().playBossAnimationSound();
}

void GameScene::toogleChangeBar()
{
	player->toogleChangeBar();
}

void GameScene::createNewBall(float spdX, float spdY, glm::vec2 pos)
{
	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram,spdX,spdY, true);
	ball->setPosition(glm::vec2(INIT_BALL_X_TILES * map->getTileSize(), INIT_BALL_Y_TILES * map->getTileSize()));
	ball->setTileMap(map);
	ball->setPlayer(player);
	ball->setBoss(boss);
	if (bossIsAlive)
		ball->toogleBossFight();
	balls.push_back(ball);
}

void GameScene::createNewBall(float spdX, float spdY)
{
	if (balls.empty()) {
		createNewBall(spdX, spdY, glm::vec2(INIT_BALL_X_TILES * map->getTileSize(), INIT_BALL_Y_TILES * map->getTileSize()));
		return;
	}
	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram,spdX,spdY, false);
	ball->setPosition(balls.front()->getPosition());
	ball->setTileMap(map);
	ball->setPlayer(player);
	ball->setBoss(boss);
	if (bossIsAlive)
		ball->toogleBossFight();
	balls.push_back(ball);
}

void GameScene::deleteLastBall()
{
	if(! balls.empty())
		balls.pop_back();
}

bool GameScene::createNewBullets() {
	if (player->getBonus() == Bonus::blaster && bullets.size() < 4) {
		bullet = new Bullet();
		glm::vec2 pos = player->getPosition();
		bullet->setPosition(glm::vec2(pos.x+2, pos.y - player->getSlideOffset().y));
		bullet->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		bullet->setTileMap(map);
		bullet->setBoss(boss);
		if (bossIsAlive)
			bullet->toogleBossFight();
		bullets.push_back(bullet);

		bullet = new Bullet();
		bullet->setPosition(glm::vec2(pos.x+10, pos.y - player->getSlideOffset().y));
		bullet->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		bullet->setTileMap(map);
		bullet->setBoss(boss);
		if (bossIsAlive)
			bullet->toogleBossFight();
		bullets.push_back(bullet);

		return true;
	}
	return false;
}

void GameScene::playerLosesLife()
{
	lives -= 1;
	alive = false;
	Game::instance().stopAlarmSound();

	if (lives < 0) {
		player->setDeathAnimation(true);
		Game::instance().playLoseLiveSound();
		gameIsOver();
	}
	else {
		menuMap->setLives(lives);
		player->setDeathAnimation(true);
		Game::instance().playLoseLiveSound();
	}
}

void GameScene::gameIsOver()
{
	gameOver = true;
	setUpGameOverSprite();
}

void GameScene::animateGameOver()
{
	static constexpr float initial_wait_time = 1000.f;
	static constexpr float frame_wait_time = 250.f;
	static constexpr int nAnimations = 13;
	float time_to_wait = 0.f;
	
	time_to_wait += initial_wait_time;
	if (auxTime < time_to_wait) return;
	Game::instance().stopBossSong();
	Game::instance().stopAlarmSound();
	Game::instance().playGameOverSong();

	antonioPos.x += 0.08f;
	antonioSprite->setPosition(antonioPos);

	for (int i = 0; i < nAnimations; ++i) {
		gameOverAnimation = 0;
		antonioAnimation = 0;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 1;
		antonioAnimation = 1;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 2;
		antonioAnimation = 2;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 3;
		antonioAnimation = 3;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
	}

	Game::instance().stopGameOverSong();
	gameOverAnimation = finished;

	return;
}

void GameScene::animateWin()
{
	static constexpr float initial_wait_time = 1000.f;
	static constexpr float frame_wait_time = 100.f;
	static constexpr int nAnimations = 53;
	float time_to_wait = 0.f;

	time_to_wait += initial_wait_time;
	if (auxTime < time_to_wait) return;

	Game::instance().stopBossSong();
	Game::instance().stopAlarmSound();
	Game::instance().playWinSong();

	antonioPos.x += 0.08f;
	antonioSprite->setPosition(antonioPos);

	for (int i = 0; i < nAnimations; ++i) {
		winAnimation = 0;
		antonioAnimation = 0;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		winAnimation = 1;
		antonioAnimation = 1;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		winAnimation = 2;
		antonioAnimation = 2;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		winAnimation = 3;
		antonioAnimation = 3;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
	}

	Game::instance().stopWinSong();
	winAnimation = finished;

	return;
}

void GameScene::animateGoBoss()
{
	static constexpr float initial_wait_time = 6000.f;

	float time_to_wait = 0.f;

	time_to_wait += initial_wait_time;
	Game::instance().stopAlarmSound();

	if (auxTime < time_to_wait) return;

	goBossAnimation = finished;
	Game::instance().playBossSong();

	return;
}

void GameScene::takeGreenCard()
{
	Game::instance().playGreenCardSound();
	greenCardAnimation = starts;
	setUpGreenCardSprite();
}

bool GameScene::convertPointsToMoney()
{
	int step = 50;
	if (points >= step) {
		points -= step;
		money += step;
	}
	else {
		money += points;
		points = 0;
	}
	menuMap->setPoints(points);
	menuMap->setMoney(money);
	
	return points <= 0;
}

bool GameScene::changeOfRoom()
{
	Ball* ball;
	
	for (auto it = balls.begin(); it != balls.end();) {

		ball = (*it);
		int thisBallRoom = -1;

		if (ball->getPosition().y / 8 > 48)	thisBallRoom = 1;
		else if (ball->getPosition().y / 8 > 24)	thisBallRoom = 2;
		else										thisBallRoom = 3;

		// We are jumping to the next Room with this ball
		if (thisBallRoom > room) {

			room_old = room;
			room = thisBallRoom;
			menuMap->setRoom(room);

			this->ball = ball;
			balls.clear();
			balls.push_back(this->ball);

			scrolling = true;
			return true;
		}
		// This ball went down. 
		else if (thisBallRoom < room) {

			//  We must delete it if there exist other ones.
			if (balls.size() > 1)
				it = balls.erase(it);

			// We must jump to the previous room otherwise.
			else
			{
				room_old = room;
				room = thisBallRoom;
				menuMap->setRoom(room);

				scrolling = true;
				return true;
			}
		}
		else ++it;
	}

	scrolling = false;
	return false;
}

void GameScene::addLive()
{
	if (godMode) setLives(lives + 1);
}

void GameScene::subLive()
{
	if (godMode) setLives(lives - 1);
}

void GameScene::nextRoom()
{
	if (godMode && room < 3)
	{
		glm::vec2 ballPos = ball->getPosition();
		ballPos.y -= 24 * 8;
		ball->setPosition(ballPos);
	}
}

void GameScene::prevRoom()
{
	if (godMode && room > 1)
	{
		glm::vec2 ballPos = ball->getPosition();
		ballPos.y += 24 * 8;
		ball->setPosition(ballPos);
	}
}

void GameScene::openDoor()
{
	if (godMode) map->openDoor();
}

void GameScene::closeDoor()
{
	if (godMode) map->closeDoor();
}

void GameScene::toggleDeathDoor()
{
	map->toggleDeathDoor();
}

bool GameScene::inGodMode()
{
	return godMode;
}

bool GameScene::ballisDead(Ball* ball)
{
	return map->tileIsDeath((ball->getBasePositionInTiles()).y, (ball->getBasePositionInTiles()).x); 
}

bool GameScene::lastBallisDead()
{
	if (balls.empty()) return true;

	for (auto it = balls.begin(); it != balls.end();)
		if (!(*it)->getMagnet() && ballisDead(*it))	it = balls.erase(it);
		else										++it;

	return balls.empty();
}

bool GameScene::checkBallSlide()
{
	for (Ball* ball : balls) {
		if (ball->getMagnet())
			return true;
	}
	return false;
}

void GameScene::startBank()
{
	string path;
	if (bank < 10)	path = "levels/BANK_0" + to_string(bank) + ".txt";
	else			path = "levels/BANK_"  + to_string(bank) + ".txt";
	
	map = TileMap::createTileMap(path.c_str(), glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map->setBank(bank);
	map->setRoom(room);

	menuMap = MenuTileMap::createTileMap("levels/menu.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	menuMap->setBank(bank);
	menuMap->setRoom(room);
	menuMap->setLives(lives);
	menuMap->setMoney(money);
	menuMap->setPoints(points);
	menuMap->setLine(" CASUAL ", " PLAYER ");

	restartPlayerBallBank();
	
	greenCardAnimation = waiting;
	winAnimation = starts;
	gameOverAnimation = starts;
	goBossAnimation = starts;
}

void GameScene::startBoss()
{
	string path;
	if (bank < 10)	path = "levels/BOSS_0" + to_string(bank) + ".txt";
	else			path = "levels/BOSS_"  + to_string(bank) + ".txt";

	map = TileMap::createTileMap(path.c_str(), glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map->setBank(bank);
	map->setRoom(room);

	menuMap = MenuTileMap::createTileMap("levels/menu.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	menuMap->setBank(bank);
	menuMap->setRoom(room);
	menuMap->setLives(lives);
	menuMap->setMoney(money);
	menuMap->setPoints(points);
	menuMap->setLine(" CASUAL ", " PLAYER ");

	bossIsAlive = true;
	restartPlayerBallBoss();
}

void GameScene::restartPlayerBallBank()
{
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTilesDisplacement(tiles_displacement);
	player->setRoom(room);
	player->setTileMap(map);

	balls.clear();
	createNewBall(1, -1, glm::vec2(INIT_BALL_X_TILES * map->getTileSize(), INIT_BALL_Y_TILES * map->getTileSize()));

	player->setPosMainBall(balls.front()->getPosition());

	bonus = new Bonus();
	bonus->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	bonus->setTileMap(map);
	bonus->setPlayer(player);
	bonus->setRoom(room);
	bonus->setPosition(glm::vec2(INIT_BONUS_X_TILES * map->getTileSize(), INIT_BONUS_Y_TILES * map->getTileSize()));

	guardian = new Guardian();
	guardian->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	guardian->setTileMap(map);
	guardian->setPosition(glm::vec2(INIT_BONUS_X_TILES * map->getTileSize(), INIT_BONUS_Y_TILES * map->getTileSize()));
	guardian->setPlayer(player);
	guardian->setRoom(map->getGuardianRoom());
	
	ballOnSlide = 0;
	restarted = true;
	Game::instance().stopAlarmSound();

}

void GameScene::restartPlayerBallBoss()
{
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTilesDisplacement(tiles_displacement);
	player->setRoom(room);
	player->setTileMap(map);

	initBoss();

	balls.clear();
	createNewBall(1, -1, glm::vec2(INIT_BALL_X_TILES * map->getTileSize(), INIT_BALL_Y_TILES * map->getTileSize()));

	player->setPosMainBall(balls.front()->getPosition());

	bonus = new Bonus();
	bonus->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	bonus->setTileMap(map);
	bonus->setPlayer(player);
	bonus->setRoom(room);
	bonus->setPosition(glm::vec2(INIT_BONUS_X_TILES * map->getTileSize(), INIT_BONUS_Y_TILES * map->getTileSize()));

	guardian = new Guardian();
	guardian->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	guardian->setTileMap(map);
	guardian->setPosition(glm::vec2(INIT_BONUS_X_TILES * map->getTileSize(), INIT_BONUS_Y_TILES * map->getTileSize()));
	guardian->setPlayer(player);
	guardian->setRoom(map->getGuardianRoom());

	ballOnSlide = 0;
	restarted = true;
	Game::instance().stopAlarmSound();
}

void GameScene::initBoss()
{
	boss = new Boss();
	boss->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	boss->setTileMap(map);
	boss->setPlayer(player);
	boss->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), (INIT_PLAYER_Y_TILES - 19) * map->getTileSize()));
}

void GameScene::insertBrick(int i, int j)
{
	std::random_device rd;
	int I = 2 + (rd() % 19);
	int J = 1 + (rd() % 20);

	map->insertBrick(I, J);
}

bool GameScene::getGameOver()
{
	return !player->getDeathAnimation() && (gameOverAnimation == finished) && gameOver;
}

bool GameScene::getWin()
{
	return (winAnimation == finished) && win;
}

bool GameScene::getGoBoss()
{
	return (goBossAnimation == finished) && goBoss;
}

int GameScene::getBank()
{
	return bank;
}

int GameScene::getRoom()
{
	return room;
}

int GameScene::getLives()
{
	return lives;
}

int GameScene::getMoney()
{
	return money;
}

int GameScene::getPoints()
{
	return points;
}

void GameScene::nextBank()
{
	// standart initializations
	room = 1;
	room_old = 1;
	tiles_displacement = -48;

	// next bank modifications
	++bank;
	alive = true;
	lives += 1;

	startBank();
}

void GameScene::setBank(int b)
{
	bank = b;
}

void GameScene::setRoom(int r)
{
	room = r;
}

void GameScene::setLives(int l)
{
	if (l >= 0) lives = l;
}

void GameScene::setMoney(int m)
{
	money = m;
}

void GameScene::setPoints(int p)
{
	points = p;
}

void GameScene::setWin(bool w)
{
	win = w;
	if (w) setUpWinSprite();
}

void GameScene::setGoBoss(bool w)
{
	goBoss = w;
	if (w) setUpGoBossSprite();
}

void GameScene::toggleGodMode()
{
	godMode = !godMode;
	toggleDeathDoor();
}