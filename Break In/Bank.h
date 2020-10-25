#pragma once
#include "TileMap.h"
class Bank :
    public TileMap
{

public:

    Bank(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

    bool loadLevel(const string& levelFile);
    void loadTextures();
    void prepareArrays(const glm::vec2& minCoords, ShaderProgram& program);

private:
    int bankID;
    Texture static_tiles, plane_colors;
};

