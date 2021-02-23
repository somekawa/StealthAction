#pragma once
#include <cocos2d.h>

class GameMap;
class PoseMenu : public cocos2d::Scene
{
public:
	static cocos2d::Scene* CreatePoseMenu(Scene* scene);
	PoseMenu(Scene* scene);
	~PoseMenu();
};

