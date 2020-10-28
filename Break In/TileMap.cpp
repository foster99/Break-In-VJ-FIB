#include "TileMap.h"
using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	return map;
}

TileMap::TileMap() {

}

TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	bankID = 1;
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{

}

void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
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
	return false;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	halfTexel = glm::vec2(0.125f / tilesheet.width(), 0.125f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = mapita[i][j].id;
			
			if (tile != 0) {
			
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);

				
				switch (char(tile)) {
				case brickRed:
					texCoordTile[0] = glm::vec2(float((tile - 1) % 2) / tilesheetSize.x, float((tile - 1) / 2) / tilesheetSize.y);
					texCoordTile[1] = texCoordTile[0] + tileTexSize;
				default:
					texCoordTile[0] = glm::vec2(float((tile - 1) % 8) / tilesheetSize.x, float((tile - 1) / 8) / tilesheetSize.y);
					texCoordTile[1] = texCoordTile[0] + tileTexSize;
					break;
				}

				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				
				
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

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
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

voxel TileMap::tileInfo(char tile, int i, int j)
{
	voxel temp;

	temp.id = 0;
	temp.resistance = -1;

	switch (tile)
	{
	case wall:
		temp.id = bankID;
		break;
	case black:
		temp.id = 38;
		break;

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		temp.id = 80 + tile - '0';
		break;

	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
		temp.id = 48 + tile - 'A';
		break;

	case '.':
		temp.id = 74;
		break;

	case ':':
		temp.id = 75;
		break;

	default:
		temp.id = tilesheetSize.x + 2 * (bankID - 1) + ((j + 1) % 2) + tilesheetSize.x * ((i + 1) % 2);
		temp.resistance = 0;
		break;

	}

	return temp;
}

bool TileMap::tileIsSolid(int i, int j)
{
	return mapita[i][j].resistance != 0;
}


bool outOfRangeLeft(int c)	{ return  c < 1; }
bool outOfRangeRight(int c) { return 23 < c; }
bool outOfRangeUp(int c)	{ return  c < 1; }
bool outOfRangeDown(int c)	{ return 23 < c; }



























