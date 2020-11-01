#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "Tile.h"

// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.

class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	TileMap();
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	~TileMap();

	void render(); 
	void free();
	
	int getTileSize() const { return tileSize; }
	int getMapSizeX() const { return mapSize.x;  }
	int getMapSizeY() const { return mapSize.y / Nrooms; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, float *posX, int speed);
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, float *posX, int speed);
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, float *posY, int speed);
	bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, float *posY, int speed);

	bool tileIsSolid(int i, int j);
	bool loadLevel(const string &levelFile);
	void loadTile(char c, int i, int j);
	void loadTextures();
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);
	void prepareDynamicArrays();

protected:

	static constexpr char wall = '*';
	static constexpr char black = '#';

	static constexpr char brickRed		= 'R';
	static constexpr char brickGreen	= 'G';
	static constexpr char brickBlue		= 'B';
	static constexpr char brickLow		= 'L';
	static constexpr char brickHigh		= 'H';
	static constexpr char door			= 'D';
	static constexpr char alarm			= 'A';
	static constexpr char key			= 'K';
	static constexpr char coin			= 'C';
	static constexpr char moneyBag		= 'M';
	static constexpr char outCard		= 'O';
	static constexpr char blueSpheres	= 'S';
	
	GLuint vaoStatic, vaoDynamic;
	GLuint vboStatic, vboDynamic;
	GLint staticPosLocation, staticTexCoordLocation, dynamicPosLocation, dynamicTexCoordLocation;

	glm::vec2 minCoords;
	ShaderProgram program;

	// TileMap info
	glm::ivec2 position, mapSize, staticTilesheetSize, dynamicTilesheetSize;
	int tileSize, blockSize;
	Texture staticTilesheet, dynamicTilesheet;
	glm::vec2 staticTileTexSize, dynamicTileTexSize;
	bool firstDynamic;
	// Bank info
	int bankID, Nrooms;
	vector<vector<Tile>> mapita;

	//vector<vector<int>> solids;

};


#endif // _TILE_MAP_INCLUDE


