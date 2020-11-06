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

	void setRoom(int room_) { currRoom = room_; };
	void setBank(int bank_) { currBank = bank_; };

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, float *posX, int speedOg, float& modifierX);
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, float *posX, int speedOg, float& modifierX);
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, float *posY, int speedOg, float& modifierY);
	bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, float *posY, int speedOg, float& modifierY);

	bool tileIsSolid(int i, int j);
	bool tileIsKey(int i, int j);
	bool tileIsDeath(int i, int j);

	void deleteKey(int i, int j);

	bool loadLevel(const string &levelFile);
	void loadTile(char c, int i, int j);
	void loadTextures();
	void prepareStaticArrays();
	void prepareDynamicArrays();

protected:

	// Static Tiles
	static constexpr char wall			= '*';
	static constexpr char black			= '#';

	// Dynamic Tiles
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
	
	// Progam Variables
	glm::vec2 minCoords;
	ShaderProgram program;
	
	// OpenGL Variables
	GLuint vaoStatic, vaoDynamic;
	GLuint vboStatic, vboDynamic;
	GLint staticPosLocation, staticTexCoordLocation, dynamicPosLocation, dynamicTexCoordLocation;
	bool first_time_we_prepare_Arrays;

	// TileMap info
	int tileSize;
	int blockSize;
	glm::ivec2 mapSize;

	// Static Tiles Texture Variables
	Texture staticTilesheet;
	glm::vec2 staticTileTexSize;
	glm::ivec2 staticTilesheetSize;

	// Dynamic Tiles Texture Variables
	Texture dynamicTilesheet;
	glm::vec2 dynamicTileTexSize;
	glm::ivec2 dynamicTilesheetSize;

	// Current Bank Info
	int currBank;
	int currRoom;
	int Nrooms;
	vector<vector<Tile>> mapita;
};


#endif // _TILE_MAP_INCLUDE


