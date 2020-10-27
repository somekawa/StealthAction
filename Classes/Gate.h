#pragma once
#include "cocos2d.h"
class Gate : public cocos2d::Sprite
{
public:
	Gate(cocos2d::Vec2 pos);
	~Gate() = default;

	static Gate* CreateGate(cocos2d::Vec2 pos);
private:
	cocos2d::Vec2 pos_;


};

