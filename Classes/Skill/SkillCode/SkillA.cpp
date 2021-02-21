#include <cmath>
#include "Generator/Geometry.h"
#include "Skill/SkillMng.h"
#include "SkillA.h"

USING_NS_CC;

SkillA::SkillA(SkillBase* ptr)
{
	pos_ = ptr->GetPlayerPos();
	effectData_.size = Size(180, 180);
	// ﾌﾟﾚｲﾔｰの方向の取得
	auto direction = ptr->GetPlayerDirection();

	Vec2 offset = {0.0f,0.0f};
	bool flip = false;
	if (direction == Direction::Right)
	{
		flip = false;
		offset = Vec2(50.0f, 20.0f);
		speed_ = 5.0f;
	}
	if (direction == Direction::Left)
	{
		flip = true;
		offset = Vec2(-50.0f, 20.0f);
		speed_ = -5.0f;
	}
	// ｴﾌｪｸﾄを作成して、自身のｴﾌｪｸﾄ画像に格納
	fx_ = lpEffectMng.PickUp("magic", Vec2{ 0.0f,0.0f }, pos_+offset, Vec2(0,0), 4, 0.08f, flip, false,false);
}

SkillA::~SkillA()
{
}

void SkillA::UpDate(float delta)
{
	float move = speed_ * 60.0f * delta;

	if (fx_.isActive_)
	{
		//effectData_.origin = fx_.sprite_->getPosition();
		fx_.sprite_->setPositionX(fx_.sprite_->getPositionX() + move);

		// 画面端に来たら
		if (fx_.sprite_->getPosition().x <= 0 || fx_.sprite_->getPosition().x >= Director::getInstance()->getWinSize().width)
		{
			// active状態をfalseにしてvisibleを不可視にする
			fx_.isActive_ = false;
			fx_.sprite_->setVisible(false);
		}
	}
}

void SkillA::Init()
{
}

bool SkillA::GetActive(void)
{
	return false;
}

cocos2d::Vec2 SkillA::GetPlayerPos(void)
{
	return cocos2d::Vec2();
}

cocos2d::Vec2 SkillA::GetTargetPos(void)
{
	return cocos2d::Vec2();
}

cocos2d::Rect SkillA::GetEffectData(void)
{
	return effectData_;
}

SkillBase* SkillA::CreateSkillA(SkillBase* ptr)
{
	SkillBase* pRet = new(std::nothrow)SkillA(ptr);
	if (pRet)
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
