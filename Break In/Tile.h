#ifndef _TILE_INCLUDE
#define _TILE_INCLUDE


class Tile {

public:

	Tile();
	Tile(int id_, int res);
	void loadTile(char c, int i, int j, int bank, int tileSheetSizeX);

	int id = 0;
	int type = staticTile;
	int resistance = infinity;

	static constexpr int menuTile = 0;
	static constexpr int staticTile = 1;
	static constexpr int dynamicTile = 2;
	static constexpr int infinity = -1;
	
	// Static tiles
	static constexpr char wall = '*';
	static constexpr char black = '#';

	// Dynamic Tiles
	static constexpr char brickRed = 'R';
	static constexpr char brickGreen = 'G';
	static constexpr char brickBlue = 'B';
	static constexpr char brickLow = 'L';
	static constexpr char brickHigh = 'H';
	static constexpr char door = 'D';
	static constexpr char alarm = 'A';
	static constexpr char key = 'K';
	static constexpr char coin = 'C';
	static constexpr char moneyBag = 'M';
	static constexpr char outCard = 'O';
	static constexpr char blueSpheres = 'S';
};
#endif