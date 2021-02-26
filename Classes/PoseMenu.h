#pragma once
#include <cocos2d.h>

class GameMap;
class PoseMenu : public cocos2d::Scene
{
public:
	static cocos2d::Scene* CreatePoseMenu(GameMap& gameMap);
	PoseMenu(GameMap& gameMap);
	~PoseMenu();
private:
	void ButtonDraw(void);
};

