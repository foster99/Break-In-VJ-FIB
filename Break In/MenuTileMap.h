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
	void prepareCounters(vector<Sprite*>& quads, glm::ivec3& coords, const glm::vec2& minCoords, ShaderProgram& program);

	void setMoney(int m);
	void setPoints(int p);
	void setLives(int l);
	void setRoom(int r);
	void setBank(int b);
	void setLine(const string &l1, const string& l2);

	void renderModifications();
	void updateMoney();
	void updateLine();
	void updatePoints();
	void updateLives();
	void updateRoom();
	void updateBank();

private:

	string intToStringOfNDigits(int i, int Ndigits);

	Texture digits;
	Texture chars;

	// Text Sprites
	vector<Sprite*> moneyQuads;
	vector<Sprite*> line1Quads;
	vector<Sprite*> line2Quads;
	vector<Sprite*> pointsQuads;
	vector<Sprite*> livesQuads;
	vector<Sprite*> roomQuads;
	vector<Sprite*> bankQuads;

	// Contabilization values
	string line1;
	string line2;
	string money;
	string points;
	string lives;
	string room;
	string bank;
	
	// Contabilization control
	bool mod_line;
	bool mod_money;
	bool mod_points;
	bool mod_lives;
	bool mod_room;
	bool mod_bank;
	
	// Contabilization info
	glm::ivec3 moneyCoords;
	glm::ivec3 pointsCoords;
	glm::ivec3 livesCoords;
	glm::ivec3 bankCoords;
	glm::ivec3 line1Coords;
	glm::ivec3 line2Coords;
	glm::ivec3 roomCoords;
};
#endif

