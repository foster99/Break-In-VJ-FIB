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
		// DYNAMIC TILES

	case brickHigh:
		id = 2 * 0 + (j % 2);
		resistance = 2;
		type = dynamicTile;
		break;
	case brickLow:
		id = 2 * 1 + (j % 2);
		resistance = 1;
		type = dynamicTile;
		break;
	case brickRed:
		id = 2 * 2 + (j % 2);
		resistance = 1;
		type = dynamicTile;
		break;
	case brickBlue:
		id = 2 * 3 + (j % 2);
		resistance = 1;
		type = dynamicTile;
		break;
	case brickGreen:
		id = 2 * 4 + (j % 2);
		resistance = 1;
		type = dynamicTile;
		break;
	case brickYellow:
		id = 2 * 5 + (j % 2);
		resistance = 1;
		type = dynamicTile;
		break;
	case door:
		id = 2 * 6 + (j % 2);
		resistance = infinity;
		type = dynamicTile;
		break;
	case moneyBag:
		id = 32 + 2 * 1 + (i % 2) * 16 + (j % 2);
		resistance = 1;
		type = dynamicTile;
		break;
	case coin:
		id = 32 + 2 * 2 + (i % 2) * 16 + (j % 2);
		resistance = 1;
		type = dynamicTile;
		break;
	case alarm:
		id = 32 + 2 * 3 + (i % 2) * 16 + (j % 2);
		resistance = infinity;
		type = dynamicTile;
		break;
	case key:
		id = 32 + 2 * 5 + (i % 2) * 16 + (j % 2);
		resistance = 1;
		type = dynamicTile;
		break;
	case outCard:
		id = 32 + 2 * 6 + (i % 2) * 16 + (j % 2);
		resistance = 1;
		type = dynamicTile;
		break;
	case blueSpheres:
		id = 32 + 2 * 7 + (i % 2) * 16 + (j % 2);
		resistance = 1;
		type = dynamicTile;
		break;

		// STATIC TILES
	case wall:
		id = bank;
		resistance = infinity;
		type = staticTile;
		break;

	default:
		id = tileSheetSizeX + 2 * (bank - 1) + ((j + 1) % 2) + tileSheetSizeX * ((i + 1) % 2);
		resistance = 0;
		type = staticTile;
		break;
	}

	symbol = tile;
}

void Tile::loadMenuTile(char tile, int i, int j, int bank)
{
	switch (tile)
	{
	case ' ':	id =  0;	break;
	case black:	id = 38;	break;
	case '.':	id = 74;	break;
	case ':':	id = 75;	break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	id = 80 + tile - '0';	break;
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
	case 'Z':	id = 48 + tile - 'A';	break;
	default:	throw "Caracter en la zona del menu no esperado !!"; break;
	}

	resistance = infinity;
	type = menuTile;
	symbol = tile;
}
