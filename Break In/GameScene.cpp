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
	alive = true;
	gameOver = false;
	gameOverAnimation = -1;
	win = false;
	timeToDelete = 0;

	setUpGameOverSprite();

}

void GameScene::update(int deltaTime) {

	this->Scene::update(deltaTime);

	if ((timeToDelete % 4) == 0) {
		//DELETEAR BULLETS SI DESTROY TRUE
		for (auto it = bullets.begin(); it != bullets.end();)
			if ((*it)->getDestroy())	it = bullets.erase(it);
			else					++it;

		timeToDelete = 0;
	}

	if (!alive) {									// ESTAMOS MUERTOS
		auxTime += deltaTime;
		if (auxTime > 175.f) {
			auxTime = 0;
			player->update(deltaTime);
		}
		alive = !(player->getDeathAnimation());

		if (alive) restartPlayerBall();
		else       return;
	}
	else if (gameOver) {							// GAME OVER, PERO HAY QUE MOSTRAR LA ANIMACION
		
		static constexpr float initial_wait_time	= 1000.f;
		static constexpr float frame_wait_time		= 250.f;
		float time_to_wait = 0.f;

		auxTime += deltaTime;

		// INITIAL WAIT
		time_to_wait += initial_wait_time;
		if (auxTime < time_to_wait) return;
		Game::instance().playGameOverSong();

		// FULL ANIMATION 1
		gameOverAnimation = 0;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 1;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 2;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 3;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;

		// FULL ANIMATION 2
		gameOverAnimation = 0;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 1;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 2;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 3;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;

		// FULL ANIMATION 3
		gameOverAnimation = 0;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 1;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 2;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 3;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;

		// FULL ANIMATION 4
		gameOverAnimation = 0;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 1;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 2;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 3;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;

		// FULL ANIMATION 5
		gameOverAnimation = 0;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 1;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 2;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 3;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;

		// FULL ANIMATION 6
		gameOverAnimation = 0;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 1;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 2;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 3;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;

		// FULL ANIMATION 7
		gameOverAnimation = 0;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 1;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 2;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 3;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;

		// FULL ANIMATION 8
		gameOverAnimation = 0;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 1;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 2;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 3;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;

		// FULL ANIMATION 9
		gameOverAnimation = 0;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 1;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 2;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 3;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;

		// FULL ANIMATION 10
		gameOverAnimation = 0;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 1;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 2;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 3;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;

		// FULL ANIMATION 11
		gameOverAnimation = 0;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 1;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 2;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 3;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;

		// FULL ANIMATION 12
		gameOverAnimation = 0;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 1;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 2;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;
		gameOverAnimation = 3;
		time_to_wait += frame_wait_time;
		if (auxTime < time_to_wait) return;

		Game::instance().stopGameOverSong();
		gameOverAnimation = -2;
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
	else {
		for (Ball* ball : balls) {
			if (!ball->getMagnet() && ball->update(deltaTime))
			{
				glm::ivec2 tile = ball->getLastCollision();
				
				if (tile.x < 0 || tile.y < 0) {
					Game::instance().playPlayerSound(); //  IMPACTO JODADORE
					if (player->getBonus() == Bonus::magnet || player->getBonus() == Bonus::twix)
						ball->toogleMagnet();
				}
				else {
					switch (map->tileCollision(tile[0], tile[1]))
					{
					case Tile::moneyBag:
						Game::instance().playMoneySound();
						money += 150;
						points += 200;
						break;

					case Tile::coin:
						Game::instance().playBrickSound();
						money += 50;
						points += 125;
						break;

					case Tile::blueSpheres:
						Game::instance().playMoneySound();
						money += 300;
						points += 333;
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
						Game::instance().playGreenCardSound();
						points += 100;
						break;

					case Tile::alarm:
						points += 100;
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
			if (!bullet->getDestroy() && bullet->update(deltaTime)) {
				glm::ivec2 tile = bullet->getLastCollision();

				switch (map->tileCollisionB(tile[0], tile[1]))
				{

				case Tile::brickBlue:
				case Tile::brickRed:
				case Tile::brickYellow:
				case Tile::brickGreen:
				case Tile::brickLow:
					Game::instance().playBrickSound();
					points += 100;
					break;
				case Tile::outCard:
					Game::instance().playGreenCardSound();
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
		player->setRoom(room);
		player->setTilesDisplacement(tiles_displacement);
		player->setPosMainBall(balls.front()->getPosition());
	}

	if (bonus->update(deltaTime)) {

		if (bonus->getActiveBonus() == Bonus::multipleBall) {
			createNewBall(-0.8,1);
			createNewBall(0.8, 1);
		}
		else player->setBonus(bonus->getActiveBonus());
		
		Game::instance().playBonusSound();
	}
	if (guardian->getRoom() == player->getCurrentRoom()) {
		if (guardian->update(deltaTime))
			playerLosesLife();
	}
	else {
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
	
	bool reset=false;
	//PARA CADA PELOTA MAGNETIZADA
	int lastMov = player->update(deltaTime);
	for (Ball* ball : balls) {
		if (ball->getMagnet()) {
			if (lastMov == Player::up || lastMov == Player::down || lastMov == Player::diag) {
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
	if (reset)
		ballOnSlide = 0;
	++timeToDelete;
	win = !map->moneyLeft();
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

	// Render Lateral Menu
	glm::mat4 menu_modelview = glm::translate(glm::mat4(1.f), glm::vec3(192.f, 0.f, 0.f));
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", menu_modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	menuMap->render();

	if (gameOverAnimation >= 0) {
		gameOverSprite->changeAnimation(gameOverAnimation);
		gameOverSprite->render(glm::mat4(1));
	}

}

void GameScene::setUpGameOverSprite()
{
	// SPRITE AND TEXTURE SET-UP
	gameOverTex.loadFromFile("images/gameover_trans.png", TEXTURE_PIXEL_FORMAT_RGBA);
	gameOverSprite = Sprite::createSprite(glm::ivec2(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT), glm::vec2(1.f / 4.f, 1.f), &gameOverTex, &texProgram);
	gameOverSprite->setNumberAnimations(4);

	gameOverSprite->addKeyframe(0, glm::vec2(0.f / 4.f, 0.f));
	gameOverSprite->addKeyframe(1, glm::vec2(1.f / 4.f, 0.f));
	gameOverSprite->addKeyframe(2, glm::vec2(2.f / 4.f, 0.f));
	gameOverSprite->addKeyframe(3, glm::vec2(3.f / 4.f, 0.f));

	gameOverSprite->changeAnimation(0);
	gameOverSprite->setPosition(glm::vec2(0.f));
}

void GameScene::toogleChangeBar()
{
	player->toogleChangeBar();
}

void GameScene::createNewBall(float spdX, float spdY, glm::vec2 pos)
{
	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram,spdX,spdY);
	ball->setPosition(glm::vec2(INIT_BALL_X_TILES * map->getTileSize(), INIT_BALL_Y_TILES * map->getTileSize()));
	ball->setTileMap(map);
	ball->setPlayer(player);
	balls.push_back(ball);
}

void GameScene::createNewBall(float spdX, float spdY)
{
	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram,spdX,spdY, false);
	ball->setPosition(balls.front()->getPosition());
	ball->setTileMap(map);
	ball->setPlayer(player);
	balls.push_back(ball);
}

void GameScene::deleteLastBall()
{
	if(! balls.empty())
		balls.pop_back();
}

void GameScene::createNewBullets() {
	if (player->getBonus() == Bonus::blaster && bullets.size() < 4) {
		bullet = new Bullet();
		glm::vec2 pos = player->getPosition();
		bullet->setPosition(glm::vec2(pos.x+2, pos.y - player->getSlideOffset().y));
		bullet->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		bullet->setTileMap(map);
		bullets.push_back(bullet);

		bullet = new Bullet();
		bullet->setPosition(glm::vec2(pos.x+10, pos.y - player->getSlideOffset().y));
		bullet->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		bullet->setTileMap(map);
		bullets.push_back(bullet);
	}
}

void GameScene::playerLosesLife()
{
	lives -= 1;
	alive = false;

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
	gameOverAnimation = -1;
}

bool GameScene::changeOfRoom()
{
	for (Ball* ball : balls) { 
		room_old = room;

		if (ball->getPosition().y / 8 > 48) {
			room = 1;
			menuMap->setRoom(room);
		}
		else if (ball->getPosition().y / 8 > 24) {
			room = 2;
			menuMap->setRoom(room);
		}
		else {
			room = 3;
			menuMap->setRoom(room);
		}

		scrolling = room != room_old;

		if (scrolling) {
			this->ball = ball;
			balls.clear();
			balls.push_back(this->ball);
			return true;
		}
	}

	return false;
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

bool GameScene::ballisDead(Ball* ball)
{
	return map->tileIsDeath((ball->getBasePositionInTiles()).y, (ball->getBasePositionInTiles()).x); 
}

bool GameScene::lastBallisDead()
{
	if (balls.empty()) return true;

	for (auto it = balls.begin(); it != balls.end();)
		if (!(*it)->getMagnet() && ballisDead(*it))	it = balls.erase(it);
		else					++it;

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
	if (bank < 10)	path = "levels/BANK_0" + to_string(bank) +".txt";
	else			path = "levels/BANK_" + to_string(bank) + ".txt";
	
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

	restartPlayerBall();
}

void GameScene::restartPlayerBall()
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
}

bool GameScene::getGameOver()
{
	return !player->getDeathAnimation() && (gameOverAnimation == -2) && gameOver;
}

bool GameScene::getWin()
{
	return win;
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
	lives = l;
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
}

void GameScene::toggleGodMode()
{
	godMode = !godMode;
	// possible update ????
}