#pragma once
#include "Scene.h"
#include "Player.h"
#include "Ball.h"

class GameScene : public Scene
{
public:

    GameScene();
    ~GameScene();

    // Inherited Methods
    void init();
    void update(int deltaTime);
    void render();

    void toggleGodMode();

    bool itIsALoopToRender();

    static constexpr float INIT_PLAYER_X_TILES = 1;
    static constexpr float INIT_PLAYER_Y_TILES = 1;

private:
    StaticTileMap* map;
    Player* player;
    Ball* ball;

    bool godMode;

    int points, money, bank, room;


    int loopsToRender;
    int currLoop;
};

