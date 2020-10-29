#include "Tile.h"

Tile::Tile() {}

Tile::Tile(int id_, int res) {
	id = id_;
	resistance = res;
}

void Tile::loadTile(char tile, int i, int j, int bank, int tileSheetSizeX)
{
	switch (tile)
	{
	case wall:
		id = bank;
		resistance = infinity;
		type = staticTile;
		break;
	case black:
		id = 38;
		resistance = infinity;
		type = menuTile;
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
		id = 80 + tile - '0';
		resistance = infinity;
		type = menuTile;
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
		id = 48 + tile - 'A';
		resistance = infinity;
		type = menuTile;
		break;

	case '.':
		id = 74;
		resistance = infinity;
		type = menuTile;
		break;

	case ':':
		id = 75;
		resistance = infinity;
		type = menuTile;
		break;

	default:
		id = tileSheetSizeX + 2 * (bank - 1) + ((j + 1) % 2) + tileSheetSizeX * ((i + 1) % 2);
		resistance = 0;
		type = staticTile;
		break;

	}

}
