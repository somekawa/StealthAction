#include <cmath>
#include "Generator/Geometry.h"
#include "Skill/SkillMng.h"
#include "SkillB.h"

USING_NS_CC;

SkillB::SkillB(SkillBase* ptr)
{
	// addできない
	pos_ = ptr->GetPlayerPos();
	tpos_ = ptr->GetTargetPos();
	effectData_.size = Size(192*0.6,192*0.6);
	// ﾌﾟﾚｲﾔｰの方向の取得
	auto direction = ptr->GetPlayerDirection();
	auto flip = false;
	if (direction == Direction::Right)
	{
		flip = true;
	}
	if (direction == Direction::Left)
	{
		flip = false;
	}

	auto Pos = Vec2(pos_.x * pos_.x, pos_.y * pos_.y);
	auto Tpos = Vec2(tpos_.x * tpos_.x, tpos_.y * tpos_.y);
	auto tvec = Tpos - Pos;
	auto vec = tpos_ - pos_;
	auto nvec = Vec2(1, 0);
	auto nvec2 = tvec / hypot(vec.x, vec.y);
	auto cos = lpGeometry.Dot(nvec, nvec2);
	auto sin = lpGeometry.Cross(nvec, nvec2);
	dir_ = Vec2(cos, sin);
	//dir_ = dir_.getNormalized() * 2;
	// ｴﾌｪｸﾄを作成して、自身のｴﾌｪｸﾄ画像に格納
	fx_ = lpEffectMng.PickUp("enemySpawn", Vec2{ 0.0f,0.0f },pos_,Vec2(0,0), 19, 0.08f,flip,false,true);
	fx_.sprite_->setScale(0.6f);
	//lpEffectMng.InitializePickedUpEffect("enemySpawn", pos_, dir, 19, 0.08f, flip, true);
	// ｴﾌｪｸﾄの再生
	//lpEffectMng.PlayWithLoop(fx_, "enemySpawn");

	time_ = 0.0f;

	param.removeFlg = false;
	param.activation = true;
	param.name = "enemySpawn";
	param.ct = 180.0f;
}

SkillB::~SkillB()
{
}

void SkillB::UpDate(float delta)
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

	time_ += delta;
	dir_ = dir_.getNormalized() * 7;
	fx_.sprite_->setPosition(fx_.sprite_->getPosition() + dir_);
	if (param.activation)
	{
		// 絶対値をとり、敵に衝突もしくは時間経過で消えるようにしておく
		if ((abs(tpos_.x - fx_.sprite_->getPosition().x) <= 60 && abs(tpos_.y - fx_.sprite_->getPosition().y) <= 60) || time_ >= 19 * 0.08)
		{
			// active状態をfalseにしてvisibleを不可視にする
			//fx_.isActive_ = false;
			//fx_.sprite_->setVisible(false);
			// SkillBaseのremoveFromParentの条件を満たすために切り替える
			param.activation = false;
		}
	}

	// 今のところｴﾌｪｸﾄがｱｸﾃｨﾌﾞ状態だと動かす
	//if (fx_.isActive_)
	//{
	//	effectData_.origin = fx_.sprite_->getPosition();
	//}

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

void SkillB::Init()
{
}

bool SkillB::GetActive(void)
{
	return false;
}

cocos2d::Vec2 SkillB::GetPlayerPos(void)
{
	return cocos2d::Vec2();
}

cocos2d::Vec2 SkillB::GetTargetPos(void)
{
	return cocos2d::Vec2();
}

cocos2d::Rect SkillB::GetEffectData(void)
{
	return effectData_;
}

SkillBase* SkillB::CreateSkillB(SkillBase* ptr)
{
	SkillBase* pRet = new(std::nothrow)SkillB(ptr);
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
