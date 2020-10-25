#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Bank.h"


Bank::Bank(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program) 
{
	loadLevel(levelFile);

	loadTextures();

	prepareArrays(minCoords, program);
}

bool Bank::loadLevel(const string& levelFile)
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

	sstream.str(line);
	sstream >> bankID;
	getline(fin, line);

	// READ TEXTURE PATH

	tilesheetFile = "images/static_tiles.png";
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);

	// EDGAR!! ESTAS INTENTANDO CARGAR BIEN LAS TEXTURAS DE LOS TILES. HAS CAMBIADO 
	// LOS FICHEROS DE LAS TEXTURAS, Y AHORA HAY QUE CARGAR TODAS LAS TEXTURAS
	// PARA CADA TIPO DE TILE CODIFICADO EN EL TXT
	int temp;
	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			fin.get(tile);
			
			switch (tile)
			{
			case '*':	
				temp = bankID;
				break;
			case ' ':
				temp = 16;//1 * tilesheetSize.y + 2 * (bankID - 1) + tilesheetSize.x * (i % 2) + (j % 2);
				break;
			default: temp = 0;	break;
			}
			
			map[j * mapSize.x + i] = temp;
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

	return true;
}

void Bank::loadTextures()
{
}

void Bank::prepareArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
	int tile, N = tilesheetSize.x, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	halfTexel = glm::vec2(0.125f / tilesheet.width(), 0.125f / tilesheet.height());
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if (tile != 0)
			{

				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);

				texCoordTile[0] = glm::vec2(float(tile / N) / tilesheetSize.x, float(tile % N) / tilesheetSize.y);
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
