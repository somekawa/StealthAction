#pragma once
#include <cocos2d.h>

class MapGenerator;
class MapMenu : public cocos2d::Scene
{
public:
	static cocos2d::Scene* CreateMapMenu(MapGenerator& mapGen, int nowID);
	MapMenu(MapGenerator& mapGen, int nowID);
	~MapMenu();
	
	
private:
};

