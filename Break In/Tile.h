#ifndef _TILE_INCLUDE
#define _TILE_INCLUDE


class Tile {

public:

	Tile();
	Tile(int id_, int res);
	void loadTile(char c, int i, int j, int bank, int tileSheetSizeX);
	void loadMenuTile(char c, int i, int j, int bank);

	int id_1 = 0;
	int id_2 = 0;
	char symbol = 0;
	int type = staticTile;
	int resistance = infinity;

	static constexpr int none = 0;
	static constexpr int menuTile = 1;
	static constexpr int staticTile = 2;
	static constexpr int dynamicTile = 3;
	static constexpr int infinity = -1;
	
	// Static tiles
	static constexpr char wall = '*';
	static constexpr char black = '#';
	static constexpr char death = 'X';

	// Dynamic Tiles
	static constexpr char brickRed = 'R';
	static constexpr char brickGreen = 'G';
	static constexpr char brickBlue = 'B';
	static constexpr char brickYellow = 'Y';
	static constexpr char brickLow = 'L';
	static constexpr char brickHigh = 'H';
	static constexpr char door = 'D';
	static constexpr char alarm = 'A';
	static constexpr char key = 'K';
	static constexpr char coin = 'C';
	static constexpr char moneyBag = 'M';
	static constexpr char outCard = 'O';
	static constexpr char blueSpheres = 'S';
	static constexpr char bomb1 = '1';
	static constexpr char bomb2 = '2';
};
#endif