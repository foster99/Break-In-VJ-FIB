#ifndef _GAMESCENE_INCLUDE
#define _GAMESCENE_INCLUDE

#include "Scene.h"
#include "Player.h"
#include "Ball.h"
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

    void toggleGodMode();
    void toogleChangeBar();

    static constexpr float INIT_PLAYER_X_TILES = 1;
    static constexpr float INIT_PLAYER_Y_TILES = 1;

private:
    TileMap* staticMap;
    MenuTileMap* menuMap;
    TileMap *dynamicMap;
    Player* player;
    Ball* ball;

    bool godMode;

    int points, money, bank, room, lives;

    int tiles_displacement;
    glm::mat4 displacement_mat;
    //int loopsToRender;
    //int currLoop;
};
#endif
