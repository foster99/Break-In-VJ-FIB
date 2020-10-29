#include "TileMap.h"
using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	return map;
}

TileMap::TileMap() {
	bankID = 1;

}

TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	bankID = 1;
	loadLevel(levelFile);
	loadTextures();
	prepareStaticArrays(minCoords, program);
}

TileMap::~TileMap()
{

}

void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	staticTilesheet.use();
	glBindVertexArray(vaoStatic);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
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
	mapita = vector<vector<Tile>> (mapSize.y, vector<Tile> (mapSize.x));

	for (int i = 0; i < mapSize.y; i++){
		for (int j = 0; j < mapSize.x; j++){
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

void TileMap::loadTile(char tile, int i, int j)
{
	mapita[i][j].loadTile(tile, i, j, bankID, tilesheetSize.x);
}

void TileMap::loadTextures()
{
	// staticTilesheet File
	staticTilesheet.loadFromFile("images/static_tiles.png", TEXTURE_PIXEL_FORMAT_RGBA);
	staticTilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	staticTilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	staticTilesheet.setMinFilter(GL_NEAREST);
	staticTilesheet.setMagFilter(GL_NEAREST);

	// dynamicTilesheet File
	//dynamicTilesheet.loadFromFile("images/dynamic_tiles.png", TEXTURE_PIXEL_FORMAT_RGBA);
	//dynamicTilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	//dynamicTilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	//dynamicTilesheet.setMinFilter(GL_NEAREST);
	//dynamicTilesheet.setMagFilter(GL_NEAREST);
}

void TileMap::prepareStaticArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	Tile tile;
	int nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	for(int i=0; i<mapSize.y; i++)
	{
		for(int j=0; j<(mapSize.x); j++)
		{
			tile = mapita[i][j];

			if (tile.type != Tile::none) {
			
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + j * tileSize,
									minCoords.y + i * tileSize);

				texCoordTile[0] = glm::vec2(float((tile.id) % tilesheetSize.x) / tilesheetSize.x,
											float((tile.id) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;

				//texCoordTile[0] += halfTexel;
				//texCoordTile[1] -= halfTexel;
				
				
				// First triangle
				vertices.push_back(posTile.x);				vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x);		vertices.push_back(texCoordTile[0].y);

				vertices.push_back(posTile.x + blockSize);	vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x);		vertices.push_back(texCoordTile[0].y);

				vertices.push_back(posTile.x + blockSize);	vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x);		vertices.push_back(texCoordTile[1].y);
				
				
				// Second triangle
				vertices.push_back(posTile.x);				vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x);		vertices.push_back(texCoordTile[0].y);

				vertices.push_back(posTile.x + blockSize);	vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x);		vertices.push_back(texCoordTile[1].y);

				vertices.push_back(posTile.x);				vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x);		vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vaoStatic);
	glBindVertexArray(vaoStatic);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, float *posJ, int speed) 
{
	int i0, i1;

	i0 = (pos.y + 2) / tileSize;
	i1 = (pos.y + size.y - 3) / tileSize;

	//if (j > mapSize.y) {
	//	*posX = mapSize.y * tileSize - tileSize;
	//	return true;
	//}

	for (int s = 1; s <= speed; ++s) {
		for (int i = i0; i <= i1; i++)
		{
			if (tileIsSolid(i, (pos.x - s) / tileSize))
			{
				*posJ -= s - 1;
				return true;
			}
		}
	}

	*posJ -= speed;

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, float *posJ, int speed)
{
	int i0, i1;

	i0 = (pos.y + 2) / tileSize;
	i1 = (pos.y + size.y - 3) / tileSize;
	
	//if (j > mapSize.y) {
	//	*posX = mapSize.y * tileSize - tileSize;
	//	return true;
	//}
	
	for (int s = 1; s <= speed; ++s) {
		for (int i = i0; i <= i1; i++)
		{
			if (tileIsSolid(i, (pos.x + size.x + s) / tileSize))
			{
				*posJ += s - 1;
				return true;
			}
		}
	}

	*posJ += speed;

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, float *posI, int speed)
{
	int j0, j1;
	
	j0 = (pos.x + 2) / tileSize;
	j1 = (pos.x + size.x - 3) / tileSize;

	//if (y >= mapSize.y) {
	//	*posY = mapSize.y*tileSize - size.y;
	//	return true;
	//}
	for (int s = 1; s <= speed; ++s) {
		for (int j = j0; j <= j1; j++)
		{
			if (tileIsSolid((pos.y + size.y + s) / tileSize, j))
			{
				*posI += s - 1;
				return true;
			}
		}
	}

	*posI += speed;

	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, float *posI, int speed)
{
	int j0, j1;

	j0 = (pos.x + 2) / tileSize;
	j1 = (pos.x + size.x - 3) / tileSize;

	//if (y < 0) {
	//	*posY = tileSize;
	//	return true;
	//}

	for (int s = 1; s <= speed; ++s) {

		for (int j = j0; j <= j1; j++)
		{
			if (tileIsSolid((pos.y - s)/tileSize, j))
			{
				*posI -= float(s - 1);
				return true;
			}
		}
	}

	*posI -= speed;

	return false;
}

bool TileMap::tileIsSolid(int i, int j)
{
	return mapita[i][j].resistance != 0;
}


bool outOfRangeLeft(int c)	{ return  c < 1; }
bool outOfRangeRight(int c) { return 23 < c; }
bool outOfRangeUp(int c)	{ return  c < 1; }
bool outOfRangeDown(int c)	{ return 23 < c; }



























