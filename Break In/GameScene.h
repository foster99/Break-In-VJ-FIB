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

    static constexpr float INIT_PLAYER_X_TILES = 10;
    static constexpr float INIT_PLAYER_Y_TILES = 10;

private:
    bool godMode;
    Bank* map;
    Player* player;
    Ball* ball;

    int loopsToRender;
    int actLoop;
};

