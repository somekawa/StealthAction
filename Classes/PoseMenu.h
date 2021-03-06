#pragma once
#include <cocos2d.h>

class GameMap;
class OPRT_state;
class PoseMenu : public cocos2d::Scene
{
public:
	static cocos2d::Scene* CreatePoseMenu(GameMap& gameMap);
	PoseMenu(GameMap& gameMap);
	~PoseMenu();
private:
	void ButtonDraw(void);
	void update(float delta);
	GameMap& gameMap_;
	OPRT_state* oprtState_;										// OPRT_stateのものを呼び出す
};