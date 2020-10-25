#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"


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

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, float *posX, int speed) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, float *posX, int speed) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, float *posY, int speed) const;
	bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, float *posY, int speed) const;

private:
	
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

protected:

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
	
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;

	// TileMap info
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;


	int *map;

};


#endif // _TILE_MAP_INCLUDE


