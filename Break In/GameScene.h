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

    bool playerLosesLife();
    bool ballOnDoor();

    static constexpr float INIT_PLAYER_X_TILES = 12;
    static constexpr float INIT_PLAYER_Y_TILES = 60;

private:
    TileMap* staticMap;
    MenuTileMap* menuMap;
    TileMap *dynamicMap;
    Player* player;
    Ball* ball;

    bool godMode, scroll;
    int stride, end;

    int points, money, bank, room, room_old, lives;

    int tiles_displacement;
    glm::mat4 displacement_mat;

};
#endif
