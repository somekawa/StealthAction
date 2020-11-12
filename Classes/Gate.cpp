#include "Gate.h"
#include "obj/Player.h"

USING_NS_CC;
Gate::Gate(Vec2 pos, int gateNum)
{
	gateNum_ = gateNum;
	pos_ = pos;
	own_ = Sprite::create("image/Sprites/map/gate.png");
	own_->setPosition(pos);
	own_->setAnchorPoint({ 0, 0 });
	own_->setScaleX(1);
	own_->setScaleY(2);
	this->addChild(own_);
}


Gate* Gate::CreateGate(cocos2d::Vec2 pos, int gateNum)
{
	Gate* pRet = new(std::nothrow) Gate(pos, gateNum);
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

void Gate::Update(Player& player)
{



}

int Gate::GetGateNum()
{
	return gateNum_;
}

bool Gate::IsHit(Player& player)
{
	auto gRect = own_->getBoundingBox();
	auto pRect = player.getBoundingBox();

	if (gRect.intersectsRect(pRect))
	{
		return true;
	}
	return false;
}

