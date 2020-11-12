#include "Tile.h"

Tile::Tile() {}

Tile::Tile(int id_, int res) {
	id_1 = id_;
	resistance = res;
}

void Tile::loadTile(char tile, int i, int j, int bank, int tileSheetSizeX)
{

	id_2 = tileSheetSizeX + 2 * (bank - 1) + ((j + 1) % 2) + tileSheetSizeX * ((i + 1) % 2);
	symbol = tile;

	switch (tile)
	{
	case brickHigh:
		id_1 = 2 * 0 + (j % 2);
		resistance = 2;
		type = dynamicTile;
		break;
	case brickLow:
		id_1 = 2 * 1 + (j % 2);
		resistance = 1;
		type = dynamicTile;
		break;
	case brickRed:
		id_1 = 2 * 2 + (j % 2);
		resistance = 1;
		type = dynamicTile;
		break;
	case brickBlue:
		id_1 = 2 * 3 + (j % 2);
		resistance = 1;
		type = dynamicTile;
		break;
	case brickGreen:
		id_1 = 2 * 4 + (j % 2);
		resistance = 1;
		type = dynamicTile;
		break;
	case brickYellow:
		id_1 = 2 * 5 + (j % 2);
		resistance = 1;
		type = dynamicTile;
		break;
	case moneyBag:
		id_1 = 32 + 2 * 1 + (i % 2) * 16 + (j % 2);
		resistance = 1;
		type = dynamicTile;
		break;
	case coin:
		id_1 = 32 + 2 * 2 + (i % 2) * 16 + (j % 2);
		resistance = 1;
		type = dynamicTile;
		break;
	case alarm:
		id_1 = 32 + 2 * 3 + (i % 2) * 16 + (j % 2);
		id_2 = 32 + 2 * 4 + (i % 2) * 16 + (j % 2);
		resistance = infinity;
		type = dynamicTile;
		break;
	case key:
		id_1 = 32 + 2 * 5 + (i % 2) * 16 + (j % 2);
		resistance = 1;
		type = dynamicTile;
		break;
	case outCard:
		id_1 = 32 + 2 * 6 + (i % 2) * 16 + (j % 2);
		resistance = 1;
		type = dynamicTile;
		break;
	case blueSpheres:
		id_1 = 32 + 2 * 7 + (i % 2) * 16 + (j % 2);
		resistance = 1;
		type = dynamicTile;
		break;
	case door:
		id_1 = bank;
		resistance = infinity;
		type = staticTile;
		id_2 = 2 * 6 + (j % 2);
		break;
	case bomb1:
	case bomb2:
		id_1 = 2 * 7 + (i % 2) * 16 + (j % 2);
		resistance = infinity;
		type = dynamicTile;
		break;
	case wall:
		id_1 = bank;
		resistance = infinity;
		type = staticTile;
		break;
	case death:
		id_1 = -1;
		resistance = 0;
		type = staticTile;
		break;

	default:
		id_1 = tileSheetSizeX + 2 * (bank - 1) + ((j + 1) % 2) + tileSheetSizeX * ((i + 1) % 2);
		resistance = 0;
		type = staticTile;
		break;
	}
}

void Tile::loadMenuTile(char tile, int i, int j, int bank)
{
	id_2 = 0;
	resistance = infinity;
	type = menuTile;
	symbol = tile;

	switch (tile)
	{
	case ' ':	id_1 =  0;	break;
	case black:	id_1 = 38;	break;
	case '.':	id_1 = 74;	break;
	case ':':	id_1 = 75;	break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	id_1 = 80 + tile - '0';	break;
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
	case 'Z':	id_1 = 48 + tile - 'A';	break;
	default:	throw "Caracter en la zona del menu no esperado !!"; break;
	}

}
