#ifndef _BANK_INCLUDE
#define _BANK_INCLUDE

#include "Game.h"
#include "GameScene.h"
#include "StaticTileMap.h"
#include <string>

//#include "DynamicTileMap.h"

class Bank
{

	public:

	Bank();
	Bank(int id);

	void init();

	void setGameScene(GameScene* scene);
	void setStaticMap(StaticTileMap* map);

	StaticTileMap* getStaticMap();
	std::string getFilePath();
	//void setDynamicMap(DynamicTileMap* map);
	bool tileIsSolid(int i, int j);

	private:

	int id, Nrooms;
	string password, shopName;

	//vector<vector<int>> tileResistance;

	GameScene* gameScene;
	StaticTileMap* staticMap;
	//DynamicTileMap* dynamicMap;
	
};
#endif