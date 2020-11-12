#include "TileMap.h"
#include <random>
using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	return map;
}

TileMap::TileMap() {
	currBank = 1;

}

TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords_, ShaderProgram &program_)
{
	currBank = 1;
	money = 0;
	alarmOn = false;
	loadLevel(levelFile);
	loadTextures();

	minCoords = minCoords_;
	program = program_;

	first_time_we_prepare_Arrays = true;
	prepareStaticArrays();
	prepareDynamicArrays();
	first_time_we_prepare_Arrays = false;
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

void TileMap::insertBrick(int i_, int j_)
{
	// Solo podemos insertar el columnas pares
	int i = i_;
	int j = (j_ % 2 == 0) ? j_ : j_+ 1;

	if (!(48 + 2 <= i && i <= 48 + 20 && 1 <= j && j <= 22))	// Excepcion si pintas en un sitio que no toca.
		assert(false);

	if (tileIsSpecial(i, j)) return;

	// Escoge aleatoriamente el tipo de bloque
	char tile;
	std::random_device rd;

	switch (rd() % 6)
	{
	case 1: tile = Tile::brickRed;		break;
	case 2: tile = Tile::brickBlue;		break;
	case 3: tile = Tile::brickGreen;	break;
	case 4: tile = Tile::brickYellow;	break;
	case 5: tile = Tile::brickLow;		break;
	case 0: tile = Tile::brickHigh;		break;
	}

	loadTile(tile, i, j);
	loadTile(tile, i, j+1);
}

bool TileMap::tileIsSpecial(int i, int j)
{
	switch (mapita[i][j].symbol)
	{
		case moneyBag:
		case coin:
		case alarm:
		case key:
		case outCard:
		case blueSpheres:
		case door:
		case wall:
		case death:
		case Tile::bomb1:
		case Tile::bomb2:
			return true;
	}
	return false;
}

void TileMap::insertShield1()
{
	int i = 48 + 10;
	int j = 6;

	loadTile('1', i, j);
	loadTile('1', i, j + 1);
	loadTile('1', i + 1, j + 1);
	loadTile('1', i + 1, j);


	insertBrick(i, j - 2);
	insertBrick(i + 1, j - 2);
	insertBrick(i, j + 2);
	insertBrick(i + 1, j + 2);
	insertBrick(i - 1, j);
	insertBrick(i + 2, j);
	insertBrick(i - 1, j + 2);
	insertBrick(i + 2, j + 2);
	insertBrick(i - 1, j - 2);
	insertBrick(i + 2, j - 2);

}

void TileMap::insertShield2()
{
	int i = 48 + 10;
	int j = 18;

	loadTile('1', i, j);
	loadTile('1', i, j + 1);
	loadTile('1', i + 1, j + 1);
	loadTile('1', i + 1, j);

	insertBrick(i, j - 2);
	insertBrick(i + 1, j - 2);
	insertBrick(i, j + 2);
	insertBrick(i + 1, j + 2);
	insertBrick(i - 1, j);
	insertBrick(i + 2, j);
	insertBrick(i - 1, j + 2);
	insertBrick(i + 2, j + 2);
	insertBrick(i - 1, j - 2);
	insertBrick(i + 2, j - 2);
}

void TileMap::deleteShield1()
{
	int i = 48 + 10;
	int j = 6;

	loadTile(' ', i, j);
	loadTile(' ', i, j + 1);
	loadTile(' ', i + 1, j + 1);
	loadTile(' ', i + 1, j);
}

void TileMap::deleteShield2()
{
	int i = 48 + 10;
	int j = 18;

	loadTile(' ', i, j);
	loadTile(' ', i, j + 1);
	loadTile(' ', i + 1, j + 1);
	loadTile(' ', i + 1, j);
}

int TileMap::getGuardianRoom()
{
	return guardianRoom;
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
	sstream >> currBank;
	getline(fin, line);

	sstream.str(line);
	sstream >> guardianRoom;

	// LEER MAPA
	mapita = vector<vector<Tile>> (mapSize.y + 1, vector<Tile> (mapSize.x));

	for (int i = 0; i < mapSize.y; i++){
		for (int j = 0; j < mapSize.x; j++){
			fin.get(tile);
			loadTile(tile, i, j);
			if (mapita[i][j].symbol == Tile::moneyBag || mapita[i][j].symbol == Tile::coin || mapita[i][j].symbol == Tile::blueSpheres) {
				money++;
			}
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();


	money = money / 4;

	for (int i = mapSize.y, j = 0; j < mapSize.x; j++) 
		loadTile(Tile::death, i, j);
	
	return true;
}

void TileMap::loadTile(char tile, int i, int j)
{
	mapita[i][j].loadTile(tile, i, j, currBank, staticTilesheetSize.x);
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
	int id_1;
	int nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	for(int i=0; i<mapSize.y; i++)
	{
		for(int j=0; j<(mapSize.x); j++)
		{
			id_1 = -1;
			switch (mapita[i][j].type) {
				case Tile::staticTile:	
				case Tile::menuTile:	id_1 = mapita[i][j].id_1; break;
				case Tile::dynamicTile: id_1 = mapita[i][j].id_2; break;
				case Tile::none:		break;
			}

			if (id_1 < 0)
				continue;
			
			// Non-empty tile
			nTiles++;
			posTile = glm::vec2(minCoords.x + j * tileSize,
				minCoords.y + i * tileSize);

			texCoordTile[0] = glm::vec2(float((id_1) % staticTilesheetSize.x) / staticTilesheetSize.x,
				float((id_1) / staticTilesheetSize.x) / staticTilesheetSize.y);
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

	if (nTiles <= 0) {
		return;
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
	frame = (++frame) % 6;

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

				texCoordTile[0] = glm::vec2(float((tile.id_1) % dynamicTilesheetSize.x) / dynamicTilesheetSize.x,
											float((tile.id_1) / dynamicTilesheetSize.x) / dynamicTilesheetSize.y);
				
				texCoordTile[1] = texCoordTile[0] + dynamicTileTexSize;

				if (tile.symbol == Tile::alarm && alarmOn && !frame) {
					texCoordTile[0] = glm::vec2(float((tile.id_2) % dynamicTilesheetSize.x) / dynamicTilesheetSize.x,
												float((tile.id_2) / dynamicTilesheetSize.x) / dynamicTilesheetSize.y);
					texCoordTile[1] = texCoordTile[0] + dynamicTileTexSize;
				}

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

	// OpenGL Stuff
	if (first_time_we_prepare_Arrays)	glGenVertexArrays(1, &vaoDynamic);
	
	glBindVertexArray(vaoDynamic);
	
	if (first_time_we_prepare_Arrays)	glGenBuffers(1, &vboDynamic);

	if (nTiles <= 0) return;

	glBindBuffer(GL_ARRAY_BUFFER, vboDynamic);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	if (first_time_we_prepare_Arrays) {
		dynamicPosLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
		dynamicTexCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	}	
}

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, float *posJ, int speedOg, float& modifierX, glm::ivec2& lastCollision)
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
				lastCollision = glm::ivec2(i,(pos.x-s) / tileSize);
				*posJ -= s - 1;
				return true;
			}
		}
	}

	*posJ += speedOg * modifierX;

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, float *posJ, int speedOg, float& modifierX, glm::ivec2& lastCollision)
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
				lastCollision = glm::ivec2(i, (pos.x + size.x + s)/tileSize);
				*posJ += s - 1;
				return true;
			}
		}
	}

	*posJ += speedOg * modifierX;

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, float *posI, int speedOg, float& modifierY, glm::ivec2& lastCollision)
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
				lastCollision = glm::ivec2((pos.y + size.y + s) / tileSize, j);
				*posI += s - 1;
				return true;
			}
		}
	}

	*posI += (speedOg * modifierY);

	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, float *posI, int speedOg, float& modifierY, glm::ivec2& lastCollision)
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
				lastCollision = glm::ivec2((pos.y - s) / tileSize, j);
				*posI -= float(s - 1);
				return true;
			}
		}
	}

	*posI += (speedOg * modifierY);

	return false;
}

bool TileMap::collisionMoveUpB(const glm::ivec2& pos, const glm::ivec2& size, float* posI, int speedOg, float& modifierY, glm::ivec2& lastCollision)
{
	int j0, j1;

	int speed = int(speedOg * abs(modifierY));

	j0 = (pos.x) / tileSize;
	j1 = (pos.x + size.x) / tileSize;

	for (int s = 1; s <= speed; ++s) {

		for (int j = j0; j <= j1; j++)
		{
			if (tileIsSolid((pos.y - s) / tileSize, j) || mapita[(pos.y - s) / tileSize][j].symbol == Tile::door)
			{
				lastCollision = glm::ivec2((pos.y - s) / tileSize, j);
				*posI -= float(s - 1);
				return true;
			}
		}
	}

	*posI += (speedOg * modifierY);

	return false;
}

char TileMap::ballTileCollision(int i, int j)
{
	char tile = mapita[i][j].symbol;
	
	switch (tile)
	{
	case brickBlue:
	case brickRed:
	case brickYellow:
	case brickGreen:
	case brickLow:
	case brickHigh:	
		collisionBrick(tile, i, j);
		break;

	case alarm:
		alarmOn = true;
		break;

	case key:
		openDoor();
		deleteSpecialTile(i, j, tile);
		break;

	case moneyBag:
	case coin:
	case blueSpheres:
		--money;
	case outCard:
		deleteSpecialTile(i, j, tile);
		break;

	default:
		break;
	}

	return tile;
}

char TileMap::bulletTileCollision(int i, int j)
{
	char tile = mapita[i][j].symbol;

	switch (tile)
	{
	case brickBlue:
	case brickRed:
	case brickYellow:
	case brickGreen:
	case brickLow:
	case brickHigh:
		collisionBrick(tile, i, j);
		break;

	case key:
		openDoor();
		deleteSpecialTile(i, j, tile);
		break;
	case outCard:
		deleteSpecialTile(i, j, tile);
		break;

	default:
		break;
	}

	return tile;
}

bool TileMap::tileIsSolid(int i, int j)
{
	return mapita[i][j].resistance != 0;
}

bool TileMap::tileIsKey(int i, int j)
{
	return (mapita[i][j].symbol == Tile::key);
}

bool TileMap::tileIsDeath(int i, int j)
{
 	return (mapita[i][j].symbol == Tile::death);
}

bool TileMap::alarmIsOn()
{
	return alarmOn;
}

bool TileMap::moneyLeft()
{
	return money > 0;
}

void TileMap::collisionBrick(char brick, int i, int j)
{
	switch (brick)
	{
	case brickBlue:
	case brickRed:
	case brickYellow:
	case brickGreen:
	case brickLow:
		loadTile(' ', i, j);
		if (j % 2)	loadTile(' ', i, j - 1);
		else		loadTile(' ', i, j + 1);
		break;
	case brickHigh:
		loadTile(brickLow, i, j);
		if (j % 2)	loadTile(brickLow, i, j - 1);
		else		loadTile(brickLow, i, j + 1);
		break;
	}
}

void TileMap::openDoor()
{
	for (int i_ = mapSize.y - (currRoom * mapSize.x), offset = 8, j_ = offset; j_ < mapSize.x - offset; ++j_)
	{
		loadTile(Tile::door, i_, j_);
		mapita[i_][j_].type = Tile::dynamicTile;
		mapita[i_][j_].id_1 = mapita[i_][j_].id_2;
		mapita[i_][j_].resistance = 0;
	}
}

void TileMap::closeDoor()
{
	for (int i_ = mapSize.y - (currRoom * mapSize.x), offset = 8, j_ = offset; j_ < mapSize.x - offset; ++j_)
	{
		loadTile(Tile::wall, i_, j_);
	}
}

void TileMap::toggleDeathDoor()
{
	char tile;
	int i = mapSize.y;

	if (mapita[i][0].symbol == Tile::death)	tile = Tile::wall;
	else									tile = Tile::death;

	for (int j = 0; j < mapSize.x; j++)
		loadTile(tile, i, j);
}


void TileMap::deleteSpecialTile(int i, int j, char tile)
{	
	loadTile(' ', i, j);

	if (i % 2 == 0 && j % 2 == 0) {
		loadTile(' ', i, j+1);
		loadTile(' ', i+1, j+1);	//  XO
		loadTile(' ', i+1, j);		//  OO
	}
	else if (i % 2 == 1 && j % 2 == 0) {
		loadTile(' ', i, j+1);
		loadTile(' ', i-1, j+1);	//	OO
		loadTile(' ', i-1, j);		//	XO
	}
	else if (i % 2 == 0 && j % 2 == 1) {
		loadTile(' ', i, j-1);
		loadTile(' ', i+1, j-1);	//	OX
		loadTile(' ', i+1, j);		//	OO
	}
	else if (i % 2 == 1 && j % 2 == 1) {
		loadTile(' ', i, j-1);
		loadTile(' ', i-1, j);		//	OO
		loadTile(' ', i-1, j-1);	//	OX
	}
}