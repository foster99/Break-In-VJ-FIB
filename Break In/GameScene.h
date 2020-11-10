#ifndef _GAMESCENE_INCLUDE
#define _GAMESCENE_INCLUDE

#include "Scene.h"
#include "Sprite.h"
#include "Player.h"
#include "Ball.h"
#include "Bonus.h"
#include "Guardian.h"
#include "MenuTileMap.h"
#include <list>

class GameScene :
    public Scene
{
public:

    GameScene();
    ~GameScene();

    // Inherited Methods
    void init();
    void update(int deltaTime);
    void render();
    void setUpGameOverSprite();

    void startBank();
    void restartPlayerBall();
    void playerLosesLife();
    void gameIsOver();

    bool getGameOver();
    bool getWin();
    
    int getBank();
    int getRoom();
    int getLives();
    int getMoney();
    int getPoints();
    
    void nextBank();

    void setBank(int b);
    void setRoom(int r);
    void setLives(int l);
    void setMoney(int m);
    void setPoints(int p);
    void setWin(bool w);

    void toggleGodMode();
    void toogleChangeBar();
    void createNewBall(float spdX, float spdY, glm::vec2 pos);
    void createNewBall(float spdX, float spdY);
    void deleteLastBall();

    bool changeOfRoom();
    void nextRoom();
    void prevRoom();
    bool ballisDead(Ball* ball);
    bool lastBallisDead();
    bool checkBallSlide();

    static constexpr float INIT_PLAYER_X_TILES  = 12;
    static constexpr float INIT_PLAYER_Y_TILES  = 68;
    static constexpr float INIT_BALL_X_TILES    = INIT_PLAYER_X_TILES + 1;
    static constexpr float INIT_BALL_Y_TILES    = INIT_PLAYER_Y_TILES - 2;
    static constexpr float INIT_BONUS_X_TILES   = 10;
    static constexpr float INIT_BONUS_Y_TILES   = 55;

private:

    // Tile Maps
    TileMap* map;
    MenuTileMap* menuMap;

    // GameScene entities
    Player* player;
    Ball* ball;
    list<Ball*> balls;
    Bonus* bonus;
    Guardian* guardian;

    // Title Quads
    Texture gameOverTex;
    Sprite* gameOverSprite;
    
    // Control variables
    bool godMode;
    bool alive;
    bool win;
    bool gameOver;
    int gameOverAnimation;
    bool bonusIsActive;
    float ballOnSlide;

    // Game Values
    int bank;
    int room;
    int room_old;
    int lives;
    int points;
    int money;
    
    // Scrolling + Displacement Mat
    bool scrolling;
    int stride;
    int end;
    int tiles_displacement;
    glm::mat4 displacement_mat;

};
#endif
