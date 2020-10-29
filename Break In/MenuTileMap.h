#ifndef _MENU_TILE_MAP_INCLUDE
#define _MENU_TILE_MAP_INCLUDE
#include "TileMap.h"

class MenuTileMap :
    public TileMap
{

public:
	static MenuTileMap* createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

	MenuTileMap();
	MenuTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);
	~MenuTileMap();

	bool loadLevel(const string& levelFile);
	void loadTile(char c, int i, int j);

};
#endif

