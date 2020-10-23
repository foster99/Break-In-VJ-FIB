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

private:
    bool godMode;
    TileMap* map;
    Player* player;
    Ball* ball;
};

