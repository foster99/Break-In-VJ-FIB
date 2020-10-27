#ifndef _DYNAMIC_TILE_MAP_INCLUDE
#define _DYNAMIC_TILE_MAP_INCLUDE

#include "TileMap.h"
class DynamicTileMap :
    public TileMap
{
public:

    DynamicTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

    void render() const;

    bool loadLevel(const string& levelFile);
    void loadTextures();
    void prepareArrays(const glm::vec2& minCoords, ShaderProgram& program);
    bool tileIsSolid(int x, int y);

private:
    int bankID;
    Texture static_tiles, plane_colors;
};
#endif

