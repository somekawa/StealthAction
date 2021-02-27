#pragma once
#include <cocos2d.h>

class OPRT_state;
class Guide :
    public cocos2d::Scene
{
public:
	static cocos2d::Scene* CreateGuide();
	Guide();
	~Guide();
private:
	void spriteDraw(cocos2d::Size size);
	void textDraw(cocos2d::Size size);
	void update(float delta);
	OPRT_state* oprtState_;										// OPRT_state‚Ì‚à‚Ì‚ğŒÄ‚Ño‚·
};