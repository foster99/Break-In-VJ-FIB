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
	// INITIALIZATIONS
	line1 = line2 = money = points = lives = room = bank = "";
	mod_line = mod_money = mod_points = mod_lives = mod_room = mod_bank = false;
	moneyCoords = pointsCoords = livesCoords = bankCoords = line1Coords = line2Coords = roomCoords = glm::ivec3(0);

}

void MenuTileMap::prepareCounters(vector<Sprite*> &quads, glm::ivec3 &coords, const glm::vec2& minCoords, ShaderProgram& program)
{
	for (int digit = 0; (unsigned int) digit < quads.size(); ++digit) {

		Sprite*& sprite = quads[digit];
		sprite = Sprite::createSprite(glm::ivec2(tileSize), glm::vec2(1.f / 10.f, 1.f), &digits, &program);
		sprite->setNumberAnimations(10);

		sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(1, glm::vec2(1.f / 10.f, 0.f));
		sprite->addKeyframe(2, glm::vec2(2.f / 10.f, 0.f));
		sprite->addKeyframe(3, glm::vec2(3.f / 10.f, 0.f));
		sprite->addKeyframe(4, glm::vec2(4.f / 10.f, 0.f));
		sprite->addKeyframe(5, glm::vec2(5.f / 10.f, 0.f));
		sprite->addKeyframe(6, glm::vec2(6.f / 10.f, 0.f));
		sprite->addKeyframe(7, glm::vec2(7.f / 10.f, 0.f));
		sprite->addKeyframe(8, glm::vec2(8.f / 10.f, 0.f));
		sprite->addKeyframe(9, glm::vec2(9.f / 10.f, 0.f));

		sprite->changeAnimation(1);

		sprite->setPosition(glm::vec2(float(tileSize * 24.f + minCoords.x + (coords.x + coords.z - 1 - digit) * tileSize),
			float(minCoords.y + coords.y * tileSize)));
	}

}

MenuTileMap::MenuTileMap(const string& levelFile, const glm::vec2& minCoords_, ShaderProgram& program_)
{
	currBank = 0;
	digits.loadFromFile("images/digits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	chars.loadFromFile("images/chars.png", TEXTURE_PIXEL_FORMAT_RGBA);

	minCoords = minCoords_;
	program = program_;

	loadLevel(levelFile);

	moneyQuads = vector<Sprite*>(moneyCoords.z, NULL);
	prepareCounters(moneyQuads, moneyCoords, minCoords, program);
	
	pointsQuads = vector<Sprite*>(pointsCoords.z, NULL);
	prepareCounters(pointsQuads, pointsCoords, minCoords, program);

	livesQuads = vector<Sprite*>(livesCoords.z, NULL);
	prepareCounters(livesQuads, livesCoords, minCoords, program);

	roomQuads = vector<Sprite*>(roomCoords.z, NULL);
	prepareCounters(roomQuads, roomCoords, minCoords, program);
	
	bankQuads = vector<Sprite*>(bankCoords.z, NULL);
	prepareCounters(bankQuads, bankCoords, minCoords, program);
	
	line1Quads = vector<Sprite*>(line1Coords.z, NULL);
	line2Quads = vector<Sprite*>(line2Coords.z, NULL);
	for (int digit = 0; (unsigned int) digit < line1Quads.size(); ++digit) {

		Sprite*& sprite1 = line1Quads[digit];
		Sprite*& sprite2 = line2Quads[digit];
		sprite1 = Sprite::createSprite(glm::ivec2(tileSize), glm::vec2(1.f / 40.f, 1.f), &chars, &program);
		sprite2 = Sprite::createSprite(glm::ivec2(tileSize), glm::vec2(1.f / 40.f, 1.f), &chars, &program);

		sprite1->setNumberAnimations(40);
		sprite2->setNumberAnimations(40);

		sprite1->addKeyframe(0, glm::vec2(0.f, 0.f));
		sprite1->addKeyframe(1, glm::vec2(1.f / 40.f, 0.f));
		sprite1->addKeyframe(2, glm::vec2(2.f / 40.f, 0.f));
		sprite1->addKeyframe(3, glm::vec2(3.f / 40.f, 0.f));
		sprite1->addKeyframe(4, glm::vec2(4.f / 40.f, 0.f));
		sprite1->addKeyframe(5, glm::vec2(5.f / 40.f, 0.f));
		sprite1->addKeyframe(6, glm::vec2(6.f / 40.f, 0.f));
		sprite1->addKeyframe(7, glm::vec2(7.f / 40.f, 0.f));
		sprite1->addKeyframe(8, glm::vec2(8.f / 40.f, 0.f));
		sprite1->addKeyframe(9, glm::vec2(9.f / 40.f, 0.f));

		sprite1->addKeyframe(10, glm::vec2(10.f / 40.f, 0.f));
		sprite1->addKeyframe(11, glm::vec2(11.f / 40.f, 0.f));
		sprite1->addKeyframe(12, glm::vec2(12.f / 40.f, 0.f));
		sprite1->addKeyframe(13, glm::vec2(13.f / 40.f, 0.f));
		sprite1->addKeyframe(14, glm::vec2(14.f / 40.f, 0.f));
		sprite1->addKeyframe(15, glm::vec2(15.f / 40.f, 0.f));
		sprite1->addKeyframe(16, glm::vec2(16.f / 40.f, 0.f));
		sprite1->addKeyframe(17, glm::vec2(17.f / 40.f, 0.f));
		sprite1->addKeyframe(18, glm::vec2(18.f / 40.f, 0.f));
		sprite1->addKeyframe(19, glm::vec2(19.f / 40.f, 0.f));

		sprite1->addKeyframe(20, glm::vec2(20.f / 40.f, 0.f));
		sprite1->addKeyframe(21, glm::vec2(21.f / 40.f, 0.f));
		sprite1->addKeyframe(22, glm::vec2(22.f / 40.f, 0.f));
		sprite1->addKeyframe(23, glm::vec2(23.f / 40.f, 0.f));
		sprite1->addKeyframe(24, glm::vec2(24.f / 40.f, 0.f));
		sprite1->addKeyframe(25, glm::vec2(25.f / 40.f, 0.f));
		sprite1->addKeyframe(26, glm::vec2(26.f / 40.f, 0.f));
		sprite1->addKeyframe(27, glm::vec2(27.f / 40.f, 0.f));
		sprite1->addKeyframe(28, glm::vec2(28.f / 40.f, 0.f));
		sprite1->addKeyframe(29, glm::vec2(29.f / 40.f, 0.f));

		sprite1->addKeyframe(30, glm::vec2(30.f / 40.f, 0.f));
		sprite1->addKeyframe(31, glm::vec2(31.f / 40.f, 0.f));
		sprite1->addKeyframe(32, glm::vec2(32.f / 40.f, 0.f));
		sprite1->addKeyframe(33, glm::vec2(33.f / 40.f, 0.f));
		sprite1->addKeyframe(34, glm::vec2(34.f / 40.f, 0.f));
		sprite1->addKeyframe(35, glm::vec2(35.f / 40.f, 0.f));
		sprite1->addKeyframe(36, glm::vec2(36.f / 40.f, 0.f));
		sprite1->addKeyframe(37, glm::vec2(37.f / 40.f, 0.f));
		sprite1->addKeyframe(38, glm::vec2(38.f / 40.f, 0.f));
		sprite1->addKeyframe(39, glm::vec2(39.f / 40.f, 0.f));

		sprite2->addKeyframe(0, glm::vec2(0.f, 0.f));
		sprite2->addKeyframe(1, glm::vec2(1.f / 40.f, 0.f));
		sprite2->addKeyframe(2, glm::vec2(2.f / 40.f, 0.f));
		sprite2->addKeyframe(3, glm::vec2(3.f / 40.f, 0.f));
		sprite2->addKeyframe(4, glm::vec2(4.f / 40.f, 0.f));
		sprite2->addKeyframe(5, glm::vec2(5.f / 40.f, 0.f));
		sprite2->addKeyframe(6, glm::vec2(6.f / 40.f, 0.f));
		sprite2->addKeyframe(7, glm::vec2(7.f / 40.f, 0.f));
		sprite2->addKeyframe(8, glm::vec2(8.f / 40.f, 0.f));
		sprite2->addKeyframe(9, glm::vec2(9.f / 40.f, 0.f));

		sprite2->addKeyframe(10, glm::vec2(10.f / 40.f, 0.f));
		sprite2->addKeyframe(11, glm::vec2(11.f / 40.f, 0.f));
		sprite2->addKeyframe(12, glm::vec2(12.f / 40.f, 0.f));
		sprite2->addKeyframe(13, glm::vec2(13.f / 40.f, 0.f));
		sprite2->addKeyframe(14, glm::vec2(14.f / 40.f, 0.f));
		sprite2->addKeyframe(15, glm::vec2(15.f / 40.f, 0.f));
		sprite2->addKeyframe(16, glm::vec2(16.f / 40.f, 0.f));
		sprite2->addKeyframe(17, glm::vec2(17.f / 40.f, 0.f));
		sprite2->addKeyframe(18, glm::vec2(18.f / 40.f, 0.f));
		sprite2->addKeyframe(19, glm::vec2(19.f / 40.f, 0.f));

		sprite2->addKeyframe(20, glm::vec2(20.f / 40.f, 0.f));
		sprite2->addKeyframe(21, glm::vec2(21.f / 40.f, 0.f));
		sprite2->addKeyframe(22, glm::vec2(22.f / 40.f, 0.f));
		sprite2->addKeyframe(23, glm::vec2(23.f / 40.f, 0.f));
		sprite2->addKeyframe(24, glm::vec2(24.f / 40.f, 0.f));
		sprite2->addKeyframe(25, glm::vec2(25.f / 40.f, 0.f));
		sprite2->addKeyframe(26, glm::vec2(26.f / 40.f, 0.f));
		sprite2->addKeyframe(27, glm::vec2(27.f / 40.f, 0.f));
		sprite2->addKeyframe(28, glm::vec2(28.f / 40.f, 0.f));
		sprite2->addKeyframe(29, glm::vec2(29.f / 40.f, 0.f));

		sprite2->addKeyframe(30, glm::vec2(30.f / 40.f, 0.f));
		sprite2->addKeyframe(31, glm::vec2(31.f / 40.f, 0.f));
		sprite2->addKeyframe(32, glm::vec2(32.f / 40.f, 0.f));
		sprite2->addKeyframe(33, glm::vec2(33.f / 40.f, 0.f));
		sprite2->addKeyframe(34, glm::vec2(34.f / 40.f, 0.f));
		sprite2->addKeyframe(35, glm::vec2(35.f / 40.f, 0.f));
		sprite2->addKeyframe(36, glm::vec2(36.f / 40.f, 0.f));
		sprite2->addKeyframe(37, glm::vec2(37.f / 40.f, 0.f));
		sprite2->addKeyframe(38, glm::vec2(38.f / 40.f, 0.f));
		sprite2->addKeyframe(39, glm::vec2(39.f / 40.f, 0.f));
	   
		sprite1->setPosition(glm::vec2(float(tileSize * 24.f + minCoords.x + (line1Coords.x + line1Coords.z - 1 - digit) * tileSize), float(minCoords.y + line1Coords.y * tileSize)));
		sprite2->setPosition(glm::vec2(float(tileSize * 24.f + minCoords.x + (line2Coords.x + line2Coords.z - 1 - digit) * tileSize), float(minCoords.y + line2Coords.y * tileSize)));
	}


	setMoney(0);
	setPoints(0);
	setLives(0);
	setRoom(0);
	setBank(0);
	setLine("........", "........");

	TileMap::loadTextures();
	prepareStaticArrays();
	renderModifications();
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
	sstream >> staticTilesheetSize.x >> staticTilesheetSize.y;
	staticTileTexSize = glm::vec2(1.f / staticTilesheetSize.x, 1.f / staticTilesheetSize.y);
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
	mapita[i][j].loadMenuTile(tile, i, j, currBank);
}

void MenuTileMap::render()
{
	glEnable(GL_TEXTURE_2D);
	staticTilesheet.use();
	glBindVertexArray(vaoStatic);
	glEnableVertexAttribArray(staticPosLocation);
	glEnableVertexAttribArray(staticTexCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);

	renderModifications();
}

// Value Modifiers

string MenuTileMap::intToStringOfNDigits(int i, int Ndigits)
{

	std::stringstream ss;
	ss << std::setw(Ndigits) << std::setfill('0') << i;
	string s = ss.str();

	/*if (s.length() > unsigned int(Ndigits))
		throw "EL NUMERO TIENE MAS DIGITOS DE LOS SOLICITADOS!!";*/

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

void MenuTileMap::renderModifications()
{
	if (mod_line) {
		updateLine();
		mod_line = false;
	}
	if (mod_money) {
		updateMoney();
		mod_money = false;
	}
	if (mod_points) {
		updatePoints();
		mod_points = false;
	}
	if (mod_lives) {
		updateLives();
		mod_lives = false;
	}
	if (mod_room) {
		updateRoom();
		mod_room = false;
	}
	if (mod_bank) {
		updateBank();
		mod_bank = false;
	}

	for (auto& digit : moneyQuads) digit->render();
	for (auto& digit : pointsQuads) digit->render();
	for (auto& digit : livesQuads) digit->render();
	for (auto& digit : roomQuads) digit->render();
	for (auto& digit : bankQuads) digit->render();
	for (auto& character : line1Quads) character->render();
	for (auto& character : line2Quads) character->render();
}

void MenuTileMap::updateMoney()
{
	for (int digit = 0; (unsigned int) digit < moneyQuads.size(); ++digit) {

		Sprite*& sprite = moneyQuads[moneyCoords.z - 1 - digit];
		sprite->changeAnimation(money[digit] - '0');
	}
}

void MenuTileMap::updateLine()
{
	for (int character = 0; (unsigned int) character < line1.size(); ++character) {

		Sprite*& sprite = line1Quads[line1Coords.z - 1 - character];
		
		switch (line1[character])
		{
		case ' ':	sprite->changeAnimation(39); break;
		case black:	sprite->changeAnimation(39); break;
		case '.':	sprite->changeAnimation(('Z' - 'A') + 1);	break;
		case ':':	sprite->changeAnimation(('Z' - 'A') + 2);	break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':	sprite->changeAnimation(line1[character] + ('Z' - 'A') + 4 - '0');	break;
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
		case 'Z':	sprite->changeAnimation(line1[character] - 'A');	break;
		default:	sprite->changeAnimation(('Z' - 'A') + 3);	break; break;
		}
	}

	for (int character = 0; (unsigned int) character < line2.size(); ++character) {

		Sprite*& sprite = line2Quads[line2Coords.z - 1 - character];

		switch (line2[character])
		{
		case ' ':	sprite->changeAnimation(39); break;
		case black:	sprite->changeAnimation(39); break;
		case '.':	sprite->changeAnimation(('Z' - 'A') + 1);	break;
		case ':':	sprite->changeAnimation(('Z' - 'A') + 2);	break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':	sprite->changeAnimation(line2[character] + ('Z' - 'A') + 3 - '0');	break;
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
		case 'Z':	sprite->changeAnimation(line2[character] - 'A');	break;
		default:	sprite->changeAnimation(('Z' - 'A') + 3);	break; break;
		}
	}
}

void MenuTileMap::updatePoints()
{
	for (int digit = 0; (unsigned int) digit < pointsQuads.size(); ++digit) {

		Sprite*& sprite = pointsQuads[pointsCoords.z - 1 - digit];
		sprite->changeAnimation(points[digit] - '0');
	}
}

void MenuTileMap::updateLives()
{
	for (int digit = 0; (unsigned int) digit < livesQuads.size(); ++digit) {

		Sprite*& sprite = livesQuads[livesCoords.z - 1 - digit];
		sprite->changeAnimation(lives[digit] - '0');
	}
}

void MenuTileMap::updateRoom()
{
	for (int digit = 0; (unsigned int) digit < roomQuads.size(); ++digit) {

		Sprite*& sprite = roomQuads[roomCoords.z - 1 - digit];
		sprite->changeAnimation(room[digit] - '0');
	}
}

void MenuTileMap::updateBank()
{
	for (int digit = 0; (unsigned int) digit < bankQuads.size(); ++digit) {

		Sprite*& sprite = bankQuads[bankCoords.z - 1 - digit];
		sprite->changeAnimation(bank[digit] - '0');
	}
}
