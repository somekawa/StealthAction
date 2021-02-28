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
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

int Gate::GetGateNum()
{
	return gateNum_;
}

//bool Gate::IsHit(Player& player)
//{
//	if (!isOpen_)
//	{
//		return false;
//	}
//	if (player.GetAction() == "player_AttackFirst"
//		|| player.GetAction() == "player_AttackSecond"
//		|| player.GetAction() == "player_AttackThird")
//	{
//		return false;
//	}
//	auto gRect = own_->getBoundingBox();
//	//auto pRect = player.getBoundingBox();
//	Size charSize = { 15.0f * 3.0f,18.0f * 3.0f };
//	auto pRect = cocos2d::Rect(player.getPosition() - charSize / 2, charSize);
//
//	if (gRect.intersectsRect(pRect))
//	{
//		// ゲートをくぐるときに、キー入力情報の初期化
//		player.KeyInputClear();
//		return true;
//	}
//	return false;
//}
//
