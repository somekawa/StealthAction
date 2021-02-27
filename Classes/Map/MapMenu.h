#pragma once
#include <cocos2d.h>

class GameMap;
class OPRT_state;
class MapMenu : public cocos2d::Scene
{
public:
	static cocos2d::Scene* CreateMapMenu(GameMap& gameMap);
	MapMenu(GameMap& gameMap);
	~MapMenu();
private:
	cocos2d::RenderTexture* tex;
	void update(float delta);
	OPRT_state* oprtState_;										// OPRT_state‚Ì‚à‚Ì‚ğŒÄ‚Ño‚·
};