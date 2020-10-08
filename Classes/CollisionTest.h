#pragma once
#include "cocos2d.h"

class CollisionTest:public cocos2d::Node
{
public:
	static CollisionTest* CollisionTest::createScene(cocos2d::Vec2 pos, cocos2d::Vec2 size);
	CollisionTest();
	~CollisionTest();
	bool isHit(void);
private:
	bool init(void);
	CREATE_FUNC(CollisionTest);
	cocos2d::Vec2 pos_;
	cocos2d::Vec2 size_;
};