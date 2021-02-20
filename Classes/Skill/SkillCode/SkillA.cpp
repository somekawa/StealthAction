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
	float speed = 0.0f;
	bool flip = false;
	if (direction == Direction::Right)
	{
		flip = false;
		offset = Vec2(50.0f, 20.0f);
		speed = 5.0f;
	}
	if (direction == Direction::Left)
	{
		flip = true;
		offset = Vec2(-50.0f, 20.0f);
		speed = -5.0f;
	}

	// ｴﾌｪｸﾄを作成して、自身のｴﾌｪｸﾄ画像に格納
	fx_ = lpEffectMng.PickUp("magic", Vec2{ 0.0f,0.0f }, pos_+offset, Vec2(speed,0), 6, 0.08f, flip, true);
}

SkillA::~SkillA()
{
}

void SkillA::UpDate(float delta)
{

	// 今のところｴﾌｪｸﾄがｱｸﾃｨﾌﾞ状態だと動かす
	if (fx_.isActive_)
	{
		effectData_.origin = fx_.sprite_->getPosition();
	}

	/*この中にSkillの効果や動作を記述してください*/
	// ここでｴﾌｪｸﾄのﾎﾟｼﾞｼｮﾝの移動をやる
	// ｱﾆﾒｰｼｮﾝが終わるとｴﾌｪｸﾄのｱｸﾃｨﾌﾞ状態をfalseにしている

	bool flag = true;
	/*if (lpEffectMng.AnimEndChecker(fx_, delta))
	{
		param.activation = false;
		flag = lpSkillMng.SkillInActivate("skill_01.txt");
	}*/

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
