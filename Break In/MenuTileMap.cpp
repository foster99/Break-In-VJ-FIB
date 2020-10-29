#include "MenuTileMap.h"

MenuTileMap* MenuTileMap::createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	MenuTileMap* map = new MenuTileMap(levelFile, minCoords, program);
	return map;
}

MenuTileMap::MenuTileMap()
{

}

MenuTileMap::MenuTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	loadLevel(levelFile);
	TileMap::loadTextures();
	prepareStaticArrays(minCoords, program);
}

MenuTileMap::~MenuTileMap()
{

}

bool MenuTileMap::loadLevel(const string& levelFile)
{
	ifstream fin;
	string line;
	string staticTilesheetFile, dynamicTilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);

	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);

	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);

	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	getline(fin, line);

	sstream.str(line);
	sstream >> bankID;

	// LEER MAPA ESTATICO
	mapita = vector<vector<Tile>>(mapSize.y, vector<Tile>(mapSize.x));

	for (int i = 0; i < mapSize.y; i++) {
		for (int j = 0; j < mapSize.x; j++) {
			fin.get(tile);
			loadTile(tile, i, j);
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

	return true;
}

void MenuTileMap::loadTile(char tile, int i, int j)
{
	mapita[i][j].loadMenuTile(tile, i, j, bankID);
}