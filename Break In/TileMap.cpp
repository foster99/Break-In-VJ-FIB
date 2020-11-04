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

TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords_, ShaderProgram &program_)
{
	bankID = 1;
	loadLevel(levelFile);
	loadTextures();

	minCoords = minCoords_;
	program = program_;

	prepareStaticArrays();
	firstDynamic = true;
	prepareDynamicArrays();
}

TileMap::~TileMap()
{ 

}

void TileMap::render()
{
	glEnable(GL_TEXTURE_2D);
	staticTilesheet.use();
	glBindVertexArray(vaoStatic);
	glEnableVertexAttribArray(staticPosLocation);
	glEnableVertexAttribArray(staticTexCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);


	glEnable(GL_TEXTURE_2D);
	dynamicTilesheet.use();
	glBindVertexArray(vaoDynamic);
	glEnableVertexAttribArray(dynamicPosLocation);
	glEnableVertexAttribArray(dynamicTexCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vboStatic);
	glDeleteBuffers(1, &vboDynamic);
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

	Nrooms = mapSize.y / mapSize.x;

	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);

	sstream.str(line);
	sstream >> staticTilesheetSize.x >> staticTilesheetSize.y;
	staticTileTexSize = glm::vec2(1.f / staticTilesheetSize.x, 1.f / staticTilesheetSize.y);
	getline(fin, line);

	sstream.str(line);
	sstream >> dynamicTilesheetSize.x >> dynamicTilesheetSize.y;
	dynamicTileTexSize = glm::vec2(1.f / dynamicTilesheetSize.x, 1.f / dynamicTilesheetSize.y);
	getline(fin, line);

	sstream.str(line);
	sstream >> bankID;

	// LEER MAPA
	mapita = vector<vector<Tile>> (mapSize.y + 1, vector<Tile> (mapSize.x));

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

	for (int i = mapSize.y, j = 0; j < mapSize.x; j++) 
		loadTile(Tile::death, i, j);
	
	return true;
}

void TileMap::loadTile(char tile, int i, int j)
{
	mapita[i][j].loadTile(tile, i, j, bankID, staticTilesheetSize.x);
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
	dynamicTilesheet.loadFromFile("images/dynamic_tiles.png", TEXTURE_PIXEL_FORMAT_RGBA);
	dynamicTilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	dynamicTilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	dynamicTilesheet.setMinFilter(GL_NEAREST);
	dynamicTilesheet.setMagFilter(GL_NEAREST);
}

void TileMap::prepareStaticArrays()
{
	int id;
	int nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	for(int i=0; i<mapSize.y; i++)
	{
		for(int j=0; j<(mapSize.x); j++)
		{
			id = -1;
			switch (mapita[i][j].type) {
				case Tile::staticTile:	
				case Tile::menuTile:	id = mapita[i][j].id; break;
				case Tile::dynamicTile: id = mapita[i][j].background_id; break;
				case Tile::none:		break;
			}

			if (id < 0)
				continue;
			
			// Non-empty tile
			nTiles++;
			posTile = glm::vec2(minCoords.x + j * tileSize,
				minCoords.y + i * tileSize);

			texCoordTile[0] = glm::vec2(float((id) % staticTilesheetSize.x) / staticTilesheetSize.x,
				float((id) / staticTilesheetSize.x) / staticTilesheetSize.y);
			texCoordTile[1] = texCoordTile[0] + staticTileTexSize;

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

	
	glGenVertexArrays(1, &vaoStatic);
	glBindVertexArray(vaoStatic);
	glGenBuffers(1, &vboStatic);
	glBindBuffer(GL_ARRAY_BUFFER, vboStatic);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	staticPosLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	staticTexCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

void TileMap::prepareDynamicArrays()
{
	Tile tile;
	int nTiles = 0;
	glm::vec2 posTile, texCoordTile[2];
	vector<float> vertices;

	for (int i = 0; i < mapSize.y; i++)
	{
		for (int j = 0; j < mapSize.x; j++)
		{
			tile = mapita[i][j];

			if (tile.type == Tile::dynamicTile) {

				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + j * tileSize,
									minCoords.y + i * tileSize);


				texCoordTile[0] = glm::vec2(float((tile.id) % dynamicTilesheetSize.x) / dynamicTilesheetSize.x,
					float((tile.id) / dynamicTilesheetSize.x) / dynamicTilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + dynamicTileTexSize;

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
	if (firstDynamic)
		glGenVertexArrays(1, &vaoDynamic);
	
	glBindVertexArray(vaoDynamic);
	
	if (firstDynamic)
		glGenBuffers(1, &vboDynamic);
	glBindBuffer(GL_ARRAY_BUFFER, vboDynamic);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	if (firstDynamic) {
		dynamicPosLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
		dynamicTexCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	}	
	firstDynamic = false;
}

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, float *posJ, int speedOg, float& modifierX)
{
	int i0, i1;

	int speed = int(speedOg * abs(modifierX));

	i0 = (pos.y + 2) / tileSize;
	i1 = (pos.y + size.y - 3) / tileSize;

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

	*posJ += speedOg * modifierX;

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, float *posJ, int speedOg, float& modifierX)
{
	int i0, i1;

	int speed = int(speedOg * abs(modifierX));

	i0 = (pos.y + 2) / tileSize;
	i1 = (pos.y + size.y - 3) / tileSize;
	
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

	*posJ += speedOg * modifierX;

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, float *posI, int speedOg, float& modifierY)
{
	int j0, j1;
	int speed = int(speedOg * abs(modifierY));

	j0 = (pos.x + 2) / tileSize;
	j1 = (pos.x + size.x - 3) / tileSize;

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

	*posI += (speedOg * modifierY);

	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, float *posI, int speedOg, float& modifierY)
{
	int j0, j1;

	int speed = int(speedOg * abs(modifierY));

	j0 = (pos.x + 2) / tileSize;
	j1 = (pos.x + size.x - 3) / tileSize;

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

	*posI += (speedOg * modifierY);

	return false;
}

bool TileMap::tileIsSolid(int i, int j)
{
	return mapita[i][j].resistance != 0;
}

bool TileMap::tileIsKey(int i, int j) {

	return (mapita[i][j].symbol == Tile::key);
}

bool TileMap::tileIsDeath(int i, int j)
{

	return (mapita[i][j].symbol == Tile::death);
}

void TileMap::deleteKey(int i, int j) {

	Tile& me		= mapita[  i  ][  j  ];
	Tile& up		= mapita[i - 1][  j  ];
	Tile& down		= mapita[i + 1][  j  ];
	Tile& left		= mapita[  i  ][j - 1];
	Tile& right		= mapita[  i  ][j + 1];
	Tile& top_left	= mapita[i - 1][j - 1];
	Tile& top_right	= mapita[i - 1][j + 1];
	Tile& bot_left	= mapita[i + 1][j - 1];
	Tile& bot_right	= mapita[i + 1][j + 1];

	if (top_left.symbol == Tile::key) {
		top_left.loadTile(' ', i - 1, j - 1, bankID, dynamicTilesheetSize.x);
		      up.loadTile(' ', i - 1,     j, bankID, dynamicTilesheetSize.x);
		    left.loadTile(' ',     i, j - 1, bankID, dynamicTilesheetSize.x);
			  me.loadTile(' ',     i,     j, bankID, dynamicTilesheetSize.x);
	}
	else if (top_right.symbol == Tile::key) {
		top_right.loadTile(' ', i - 1, j + 1, bankID, dynamicTilesheetSize.x);
		       up.loadTile(' ', i - 1,     j, bankID, dynamicTilesheetSize.x);
		    right.loadTile(' ',     i, j + 1, bankID, dynamicTilesheetSize.x);
		       me.loadTile(' ',     i,     j, bankID, dynamicTilesheetSize.x);
	}
	else if (bot_left.symbol == Tile::key) {
		 bot_left.loadTile(' ', i + 1, j - 1, bankID, dynamicTilesheetSize.x);
		     down.loadTile(' ', i + 1,     j, bankID, dynamicTilesheetSize.x);
		     left.loadTile(' ',     i, j - 1, bankID, dynamicTilesheetSize.x);
		       me.loadTile(' ',     i,     j, bankID, dynamicTilesheetSize.x);
	}
	else if (bot_right.symbol == Tile::key) {
		bot_right.loadTile(' ', i + 1, j + 1, bankID, dynamicTilesheetSize.x);
		     down.loadTile(' ', i + 1,     j, bankID, dynamicTilesheetSize.x);
		    right.loadTile(' ',     i, j + 1, bankID, dynamicTilesheetSize.x);
		       me.loadTile(' ',     i,     j, bankID, dynamicTilesheetSize.x);
	}
}






















