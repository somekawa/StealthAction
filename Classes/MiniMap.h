#pragma once
#include <cocos2d.h>

class MapGenerator;
class MiniMap : public cocos2d::Scene
{
public:
	static cocos2d::Scene* CreateMiniMap(MapGenerator& mapGen, int nowID);
	MiniMap(MapGenerator& mapGen, int nowID);
	~MiniMap();
	
	
private:
};

