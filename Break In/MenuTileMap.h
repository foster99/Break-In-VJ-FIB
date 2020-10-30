#ifndef _MENU_TILE_MAP_INCLUDE
#define _MENU_TILE_MAP_INCLUDE
#include "TileMap.h"
#include "Sprite.h"
class MenuTileMap :
    public TileMap
{

public:
	static MenuTileMap* createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

	MenuTileMap();
	MenuTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);
	~MenuTileMap();

	void render();
	bool loadLevel(const string& levelFile);
	void loadTile(char c, int i, int j);

	void setMoney(int m);
	void setPoints(int p);
	void setLives(int l);
	void setRoom(int r);
	void setBank(int b);
	void setLine(const string &l1, const string& l2);

	void renderModifications(const glm::vec2& minCoords, ShaderProgram& program);
	void updateMoney(const glm::vec2& minCoords, ShaderProgram& program);
	void prepareLineArrays(const glm::vec2& minCoords, ShaderProgram& program);
	void preparePointsArrays(const glm::vec2& minCoords, ShaderProgram& program);
	void prepareLivesArrays(const glm::vec2& minCoords, ShaderProgram& program);
	void prepareRoomArrays(const glm::vec2& minCoords, ShaderProgram& program);
	void prepareBankArrays(const glm::vec2& minCoords, ShaderProgram& program);

private:

	string intToStringOfNDigits(int i, int Ndigits);

	Texture digits;
	vector<Sprite*> moneyQuads;

	// Contabilization values
	string line1, line2, money, points, lives, room, bank;
	
	// Contabilization modification control
	bool mod_line, mod_money, mod_points, mod_lives, mod_room, mod_bank;
	
	// Contabilization info
	glm::ivec3 moneyCoords, pointsCoords, livesCoords, bankCoords, line1Coords, line2Coords, roomCoords;
};
#endif

