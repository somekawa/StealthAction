#pragma once
#include <cocos2d.h>
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
};

