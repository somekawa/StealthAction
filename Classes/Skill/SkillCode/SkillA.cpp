// 担当場所
#include <cmath>
#include "_Debug/_DebugConOut.h"
#include "Generator/Geometry.h"
#include "Skill/SkillMng.h"
#include "SkillA.h"

USING_NS_CC;

SkillA::SkillA(SkillBase* ptr)
{
	pos_ = ptr->GetPlayerPos();
	effectData_.size = Size(100, 100);
	// プレイヤーの方向の取得
	const auto direction = ptr->GetPlayerDirection();

	Vec2 offset = {0.0f,0.0f};
	bool flip = false;
	if (direction == Direction::Right)
	{
		flip = false;
		offset = Vec2(50.0f, 20.0f);
		speed_ = 10.0f;
	}
	if (direction == Direction::Left)
	{
		flip = true;
		offset = Vec2(-50.0f, 20.0f);
		speed_ = -10.0f;
	}
	// エフェクトを作成して、自身のエフェクト画像に格納
	fx_ = lpEffectMng.PickUp("magic", Vec2(0.0f,0.0f), pos_, Vec2(0.0f,0.0f), 4, 0.08f, flip, false,true);

	param.removeFlg = false;
	param.activation = true;
	param.name = "magic";
	param.ct = 15.0f * 60.0f;		// 15秒
}

SkillA::~SkillA()
{
}

SkillBase* SkillA::CreateSkillA(SkillBase* ptr)
{
	SkillBase* pRet = new(std::nothrow)SkillA(ptr);
	if (pRet)
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

void SkillA::UpDate(float delta)
{
	if (!param.activation)
	{
		if (param.ct > 0)
		{
			param.ct--;
		}
		else
		{
			param.ct = 0;
			param.removeFlg = true;
		}
	}

	if (param.activation)
	{
		fx_.sprite_->setPosition(fx_.sprite_->getPositionX() + (speed_ * 60.0f * delta) , pos_.y);
		effectData_.origin = fx_.sprite_->getPosition();
		// 画面端に来たら
		if (fx_.sprite_->getPosition().x <= 0.0f ||
			fx_.sprite_->getPosition().x >= Director::getInstance()->getWinSize().width)
		{
			param.activation = false;
		}
	}
}