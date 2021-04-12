#include "Gate.h"
#include "obj/Player.h"
#include "anim/AnimMng.h"

USING_NS_CC;
Gate::Gate(Vec2 pos, int gateNum)
{
	gateNum_ = gateNum;
	own_ = Sprite::create();
	own_->runAction(RepeatForever::create(Animate::create(lpAnimMng.GetAnimationCache("warp"))));
	own_->setPosition(pos);
	own_->setAnchorPoint({ 0, 0 });
	this->addChild(own_);
	this->setName("gate");
	this->setVisible(false);
	isEnable_ = false;
}

Gate::~Gate()
{
	CC_SAFE_RELEASE(own_);
}

Gate* Gate::CreateGate(cocos2d::Vec2 pos, int gateNum)
{
	Gate* pRet = new(std::nothrow) Gate(pos, gateNum);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

int Gate::GetGateNum()
{
	return gateNum_;
}