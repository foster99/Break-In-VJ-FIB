#include "MenuTileMap.h"

#include <sstream>
#include <iomanip>

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
	bankID = 0;
	digits.loadFromFile("images/digits.png", TEXTURE_PIXEL_FORMAT_RGBA);

	loadLevel(levelFile);

	moneyQuads = vector<Sprite*>(moneyCoords.z, NULL);
	for (int digit = 0; digit < moneyQuads.size(); ++digit) {

		Sprite* &sprite = moneyQuads[digit];
		sprite = Sprite::createSprite(glm::ivec2(tileSize), glm::vec2(1.f/10.f, 1.f), &digits, &program);
		sprite->setNumberAnimations(10);

		sprite->addKeyframe(0, glm::vec2(       0.f, 0.f));
		sprite->addKeyframe(1, glm::vec2(1.f / 10.f, 0.f));
		sprite->addKeyframe(2, glm::vec2(2.f / 10.f, 0.f));
		sprite->addKeyframe(3, glm::vec2(3.f / 10.f, 0.f));
		sprite->addKeyframe(4, glm::vec2(4.f / 10.f, 0.f));
		sprite->addKeyframe(5, glm::vec2(5.f / 10.f, 0.f));
		sprite->addKeyframe(6, glm::vec2(6.f / 10.f, 0.f));
		sprite->addKeyframe(7, glm::vec2(7.f / 10.f, 0.f));
		sprite->addKeyframe(8, glm::vec2(8.f / 10.f, 0.f));
		sprite->addKeyframe(9, glm::vec2(9.f / 10.f, 0.f));

		sprite->changeAnimation(0);
		
		sprite->setPosition(glm::vec2(	float(tileSize * 24.f + minCoords.x + (moneyCoords.x + moneyCoords.z - 1 - digit) * tileSize),
										float(minCoords.y + moneyCoords.y * tileSize)));

		//sprite->setPosition(glm::vec2(float(minCoords.x + (moneyCoords.x + moneyCoords.z - 1 - digit) * tileSize),
									  //float(minCoords.y + digit * tileSize)));
	}
		
		


	setMoney(0);
	setPoints(0);
	setLives(0);
	setRoom(0);
	setBank(0);
	setLine("........", "........");

	TileMap::loadTextures();
	prepareStaticArrays(minCoords, program);
	//renderModifications(minCoords, program);
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

	// READ MAPSIZE
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);

	// READ Tile size& block size
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);

	// READ TILESHEET SIZE
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	getline(fin, line);

	// READ MONEY COORDS
	sstream.str(line);
	sstream >> moneyCoords.y >> moneyCoords.x >> moneyCoords.z;
	getline(fin, line);

	// READ POINTS COORDS
	sstream.str(line);
	sstream >> pointsCoords.y >> pointsCoords.x >> pointsCoords.z;
	getline(fin, line);

	// READ LIVES COORDS
	sstream.str(line);
	sstream >> livesCoords.y >> livesCoords.x >> livesCoords.z;
	getline(fin, line);

	// READ BANK COORDS
	sstream.str(line);
	sstream >> bankCoords.y >> bankCoords.x >> bankCoords.z;
	getline(fin, line);

	// READ LINE 1 COORDS
	sstream.str(line);
	sstream >> line1Coords.y >> line1Coords.x >> line1Coords.z;
	getline(fin, line);

	// READ LINE 2 COORDS
	sstream.str(line);
	sstream >> line2Coords.y >> line2Coords.x >> line2Coords.z;
	getline(fin, line);

	// READ ROOM COORDS
	sstream.str(line);
	sstream >> roomCoords.y >> roomCoords.x >> roomCoords.z;
	getline(fin, line);

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

void MenuTileMap::render()
{
	this->TileMap::render();
	renderModifications((*minCoords), (*program));
}

// Value Modifiers

string MenuTileMap::intToStringOfNDigits(int i, int Ndigits)
{

	std::stringstream ss;
	ss << std::setw(Ndigits) << std::setfill('0') << i;
	string s = ss.str();

	if (s.length() > unsigned int(Ndigits))
		throw "EL NUMERO TIENE MAS DIGITOS DE LOS SOLICITADOS!!";

	return s;
}

void MenuTileMap::setMoney(int m)
{
	mod_money = true;
	money = intToStringOfNDigits(m, moneyCoords.z);
}

void MenuTileMap::setPoints(int p)
{
	mod_points = true;
	points = intToStringOfNDigits(p, pointsCoords.z);
}

void MenuTileMap::setLives(int l)
{
	mod_lives = true;
	lives = intToStringOfNDigits(l, livesCoords.z);
}

void MenuTileMap::setRoom(int r)
{
	mod_room = true;
	room = intToStringOfNDigits(r, roomCoords.z);
}

void MenuTileMap::setBank(int b)
{
	mod_bank = true;
	bank = intToStringOfNDigits(b, bankCoords.z);
}

void MenuTileMap::setLine(const string& l1, const string& l2)
{
	mod_line = true;
	line1 = l1;
	line2 = l2;
}

// Array Modification
int digitToPosInTexCoord(char tile)
{
	return 80 + tile - '0';
}

void MenuTileMap::renderModifications(const glm::vec2& minCoords, ShaderProgram& program)
{
	if (mod_line) {
		prepareLineArrays(minCoords, program);
		mod_line = false;
	}
	if (mod_money) {
		updateMoney(minCoords, program);
		mod_money = false;
	}
	if (mod_points) {
		preparePointsArrays(minCoords, program);
		mod_points = false;
	}
	if (mod_lives) {
		prepareLivesArrays(minCoords, program);
		mod_lives = false;
	}
	if (mod_room) {
		prepareRoomArrays(minCoords, program);
		mod_room = false;
	}
	if (mod_bank) {
		prepareBankArrays(minCoords, program);
		mod_bank = false;
	}

	for (auto& digit : moneyQuads) digit->render();
}

void MenuTileMap::updateMoney(const glm::vec2& minCoords, ShaderProgram& program)
{
	for (int digit = 0; digit < moneyQuads.size(); ++digit) {

		Sprite*& sprite = moneyQuads[moneyCoords.z - 1 - digit];
		sprite->changeAnimation(money[digit] - '0');
	}

	/* INTENTO DE VERSION CON VAOs y VBOs*/

	//int nDigits = moneyCoords.z;
	//int digit = moneyCoords.y;
	//int j = moneyCoords.x + nDigits;

	//glm::vec2 posTile, texCoordTile[2];
	//vector<float> vertices;

	//for (int digit = 0; digit < nDigits; digit++)
	//{
	//	posTile.x = minCoords.x + (j - digit) * tileSize;
	//	posTile.y = minCoords.y + digit * tileSize;

	//	int tex_pos = digitToPosInTexCoord(money[j - digit]);

	//	texCoordTile[0] = glm::vec2(float((tex_pos) % tilesheetSize.x) / tilesheetSize.x,
	//								float((tex_pos) / tilesheetSize.x) / tilesheetSize.y);
	//	texCoordTile[1] = texCoordTile[0] + tileTexSize;


	//	// First triangle
	//	vertices.push_back(posTile.x);				vertices.push_back(posTile.y);
	//	vertices.push_back(texCoordTile[0].x);		vertices.push_back(texCoordTile[0].y);

	//	vertices.push_back(posTile.x + blockSize);	vertices.push_back(posTile.y);
	//	vertices.push_back(texCoordTile[1].x);		vertices.push_back(texCoordTile[0].y);

	//	vertices.push_back(posTile.x + blockSize);	vertices.push_back(posTile.y + blockSize);
	//	vertices.push_back(texCoordTile[1].x);		vertices.push_back(texCoordTile[1].y);


	//	// Second triangle
	//	vertices.push_back(posTile.x);				vertices.push_back(posTile.y);
	//	vertices.push_back(texCoordTile[0].x);		vertices.push_back(texCoordTile[0].y);

	//	vertices.push_back(posTile.x + blockSize);	vertices.push_back(posTile.y + blockSize);
	//	vertices.push_back(texCoordTile[1].x);		vertices.push_back(texCoordTile[1].y);

	//	vertices.push_back(posTile.x);				vertices.push_back(posTile.y + blockSize);
	//	vertices.push_back(texCoordTile[0].x);		vertices.push_back(texCoordTile[1].y);

	//}

	//glGenVertexArrays(1, &vaoMoney);
	//glBindVertexArray(vboMoney);
	//glGenBuffers(1, &vboMoney);
	//glBindBuffer(GL_ARRAY_BUFFER, vboMoney);
	//glBufferData(GL_ARRAY_BUFFER, 24 * nDigits * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
	//posLocationMoney = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	//texCoordLocationMoney = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void MenuTileMap::prepareLineArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
}

void MenuTileMap::preparePointsArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
}

void MenuTileMap::prepareLivesArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
}

void MenuTileMap::prepareRoomArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
}

void MenuTileMap::prepareBankArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
}
