#include "Gate.h"

USING_NS_CC;
Gate::Gate(Vec2 pos)
{
	pos_ = pos;
	Sprite* sp = Sprite::create("image/Sprites/map/gate.png");
	sp->setPosition(pos);
	this->addChild(sp);
}


Gate* Gate::CreateGate(cocos2d::Vec2 pos)
{
	Gate* pRet = new(std::nothrow) Gate(pos);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}
