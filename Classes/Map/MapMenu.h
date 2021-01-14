#pragma once
#include <cocos2d.h>

class GameMap;
class MapMenu : public cocos2d::Scene
{
public:
	static cocos2d::Scene* CreateMapMenu(GameMap& gameMap);
	MapMenu(GameMap& gameMap);
	~MapMenu();
	
	
private:
};

