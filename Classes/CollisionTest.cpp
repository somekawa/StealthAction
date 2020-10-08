#include "CollisionTest.h"

USING_NS_CC;

CollisionTest* CollisionTest::createScene(cocos2d::Vec2 pos, cocos2d::Vec2 size)
{
	CollisionTest* pRet = new(std::nothrow) CollisionTest();
	if (pRet->init())
	{
		// ŒÄ‚Ño‚µ
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

CollisionTest::CollisionTest()
{
}

CollisionTest::~CollisionTest()
{
}

bool CollisionTest::isHit(void)
{
	return true;
}

bool CollisionTest::init(void)
{
	return true;
}
