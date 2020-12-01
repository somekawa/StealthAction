#pragma once
#include <cocos2d.h>
class BaseScene : public cocos2d::Scene
{
public:
	virtual bool init()override = 0;
	virtual void update(float delta)override = 0;
};

