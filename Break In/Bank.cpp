#include "Bank.h"

Bank::Bank()
{
	id = 0;
	Nrooms = 0;
	staticMap = NULL;
}

Bank::Bank(int id)
{
	this->id = id;
}

void Bank::init()
{
	
}

void Bank::setGameScene(GameScene* scene)
{
	gameScene = scene;
}

void Bank::setStaticMap(StaticTileMap* map)
{
	staticMap = map;
}

StaticTileMap* Bank::getStaticMap()
{
	return staticMap;
}

string Bank::getFilePath()
{
	if (id < 10)
		return string("levels/bank_0" + std::to_string(id) + ".txt");

	return string("levels/bank_" + std::to_string(id) + ".txt");
}

bool Bank::tileIsSolid(int i, int j)
{
	if (staticMap->tileIsSolid(i, j))
		return true;
	//if (dynamicMap->tileIsSolid(i, j))
	//	return true;
	return false;
}

//void Bank::setDynamicMap(DynamicTileMap* map)
//{
//	dynamicMap = map;
//}
