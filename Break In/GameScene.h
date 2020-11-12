#ifndef _GAMESCENE_INCLUDE
#define _GAMESCENE_INCLUDE

#include "Scene.h"
#include "Sprite.h"
#include "Player.h"
#include "Ball.h"
#include "Bonus.h"
#include "Guardian.h"
#include "MenuTileMap.h"
#include "Bullet.h"
#include "Boss.h"
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

    void refreshBoss();
    void insertBrick(int i, int j);

    void setUpGameOverSprite();
    void setUpWinSprite();
    void setUpGreenCardSprite();
    void setUpGoBossSprite();
    void setUpEndGameSprite();

    void startBank();
    void startBoss();
    void restartPlayerBallBank();
    void restartPlayerBallBoss();
    void playerLosesLife();
    void gameIsOver();

    void animateGameOver();
    void animateWin();
    void animateGoBoss();
    void animateEndGame();
    void setEndGame(bool e);

    bool getGameOver();
    bool getWin();
    bool getGoBoss();

    bool endAnimationIsActive();


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
    void setGoBoss(bool w);

    void toggleGodMode();
    void toogleChangeBar();
    void createNewBall(float spdX, float spdY, glm::vec2 pos);
    void createNewBall(float spdX, float spdY);
    void deleteLastBall();
    bool createNewBullets();
    
    bool convertPointsToMoney();
    void takeGreenCard();
    bool changeOfRoom();
    bool ballisDead(Ball* ball);
    bool lastBallisDead();
    bool checkBallSlide();

    // GOD MODE
    void addLive();
    void subLive();
    void nextRoom();
    void prevRoom();
    void openDoor();
    void closeDoor();
    void toggleDeathDoor(bool die);
    bool inGodMode();

    static constexpr float INIT_PLAYER_X_TILES  = 12;
    static constexpr float INIT_PLAYER_Y_TILES  = 68;
    static constexpr float INIT_BALL_X_TILES    = INIT_PLAYER_X_TILES + 1;
    static constexpr float INIT_BALL_Y_TILES    = INIT_PLAYER_Y_TILES - 2;
    static constexpr float INIT_BONUS_X_TILES   = 10;
    static constexpr float INIT_BONUS_Y_TILES   = 55;

    static constexpr float TILE_COLLISION = 0;
    static constexpr float PLAYER_COLLISION = 1;
    static constexpr float BOSS_COLLISION = 2;


private:

    // Tile Maps
    TileMap* map;
    MenuTileMap* menuMap;

    // GameScene entities
    Player* player;
    Ball* ball;
    list<Ball*> balls;
    Bullet* bullet;
    list<Bullet*> bullets;
    Bonus* bonus;
    Guardian* guardian;
    Boss* boss;

    static constexpr int waiting = 0;
    static constexpr int starts = -1;
    static constexpr int finished = -2;

    // GameOver Animation
    Texture gameOverTex;
    Sprite* gameOverSprite;
    int gameOverAnimation;

    // Win Animation
    Texture winTex;
    Sprite* winSprite;
    int winAnimation;

    // Boss Animation
    Texture goBossTex;
    Sprite* goBossSprite;
    int goBossAnimation;

    // Antonio Animation
    Texture antonioTex;
    Sprite* antonioSprite;
    glm::vec2 antonioPos;
    int antonioAnimation;

    // GodModeQuad
    Texture godModeTex;
    Sprite* godModeSprite;
    
    // END GAME
    Texture endGameTex;
    Sprite* endGameSprite;
    bool endGame;
    int endGameAnimation;

    // GreenCard
    Texture greenCardTex;
    Sprite* greenCardSprite;
    int greenCardAnimation;

    // Boss Features
    bool weMustCreateTheBoss;
    bool bossIsAlive;

    // Control variables
    bool restarted;
    bool godMode;
    bool alive;
    bool win;
    bool goBoss;
    bool gameOver;
    bool bonusIsActive;
    float ballOnSlide;
    int timeToDelete;
    
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
