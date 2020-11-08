#ifndef _GAMESCENE_INCLUDE
#define _GAMESCENE_INCLUDE

#include "Scene.h"
#include "Player.h"
#include "Ball.h"
#include "Bonus.h"
#include "Guardian.h"
#include "MenuTileMap.h"

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

    void toggleGodMode();
    void toogleChangeBar();

    bool changeOfRoom();
    void nextRoom();
    void prevRoom();
    bool lastBallisDead();

    static constexpr float INIT_PLAYER_X_TILES = 12;
    static constexpr float INIT_PLAYER_Y_TILES = 60;
    static constexpr float INIT_BALL_X_TILES = INIT_PLAYER_X_TILES;
    static constexpr float INIT_BALL_Y_TILES = INIT_PLAYER_Y_TILES - 1;
    static constexpr float INIT_BONUS_X_TILES = 10;
    static constexpr float INIT_BONUS_Y_TILES = 55;

private:

    // Tile Maps
    TileMap* map;
    MenuTileMap* menuMap;

    // GameScene entities
    Player* player;
    Ball* ball;
    Bonus* bonus;
    Guardian* guardian;

    // Control variables
    bool godMode;
    bool alive;
    bool win;
    bool gameOver;
    bool bonusIsActive;

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
