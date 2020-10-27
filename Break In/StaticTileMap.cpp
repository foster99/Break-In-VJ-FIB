#include "StaticTileMap.h"


StaticTileMap::StaticTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program) 
{
	loadLevel(levelFile);

	loadTextures();

	prepareArrays(minCoords, program);
}

void StaticTileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

bool StaticTileMap::loadLevel(const string& levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
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

	// READ TEXTURE PATH

	tilesheetFile = "images/static_tiles.png";
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);

	int temp;
	solids = vector<vector<int>>(1024, vector<int>(1024, -1));
	map = new int[mapSize.x * mapSize.y];
	for (int i = 0; i < mapSize.y; i++)
	{
		for (int j = 0; j < mapSize.x; j++)
		{
			// Fila i, Columna j

			fin.get(tile);
			
			switch (tile)
			{
			case wall:
				temp = bankID;
				break;
			case black:
				temp = 38;
				break;
			case ':':
				temp = 75;
				break;
			case '.':
				temp = 74;
				break;
			default:

				if ('A' <= tile && tile <= 'Z') {	// Char
					temp = 48 + tile - 'A';
					break;
				}

				if ('0' <= tile && tile <= '9') {	// Number
					temp = 80 + tile - '0';
					break;
				}
				
				solids[i + 256][j + 256] = 0;		// Background (non-solid)
				temp = tilesheetSize.x + 2 * (bankID - 1) + ((j + 1) % 2) + tilesheetSize.x * ((i + 1) % 2);
				break;

			}

			map[i * mapSize.x + j] = temp;
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

	return true;
}

void StaticTileMap::loadTextures()
{
}

bool StaticTileMap::tileIsSolid(int i, int j)
{
	return solids[i + 256][j + 256] != 0;
}

void StaticTileMap::prepareArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
	int tile, N = tilesheetSize.x, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	halfTexel = glm::vec2((1.f/float(tileSize))/ tilesheet.width(), (1.f / float(tileSize)) / tilesheet.height());
	for (int i = 0; i < mapSize.y; i++)
	{
		for (int j = 0; j < mapSize.x; j++)
		{
			tile = map[i * mapSize.x + j];
			if (tile != 0)
			{

				// Non-empty tile
				nTiles++;
				posTile = glm::vec2( minCoords.x + j * tileSize,
									 minCoords.y + i * tileSize);

				texCoordTile[0] = glm::vec2(float(tile % tilesheetSize.x) / float(tilesheetSize.x),
											float(tile / tilesheetSize.x) / float(tilesheetSize.y));
				//texCoordTile[0] = glm::vec2(1.f/16.f, 0.f/16.f);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;

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
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}
