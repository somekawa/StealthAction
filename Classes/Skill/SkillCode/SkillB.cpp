// 担当場所
#include <cmath>
#include "Generator/Geometry.h"
#include "Skill/SkillMng.h"
#include "SkillB.h"

USING_NS_CC;

SkillB::SkillB(SkillBase* ptr)
{
	pos_ = ptr->GetPlayerPos();
	tpos_ = ptr->GetTargetPos();
	effectData_.size = Size(192.0f * 0.6f,192.0f * 0.6f);
	// プレイヤーの方向の取得
	const auto direction = ptr->GetPlayerDirection();
	bool flip = (direction == Direction::Right ? true : false);

	const auto Pos = Vec2(pos_.x * pos_.x, pos_.y * pos_.y);
	const auto Tpos = Vec2(tpos_.x * tpos_.x, tpos_.y * tpos_.y);
	const auto tvec = Tpos - Pos;
	const auto vec = tpos_ - pos_;
	const auto nvec = Vec2(1.0f, 0.0f);
	const auto nvec2 = tvec / hypot(vec.x, vec.y);
	const auto cos = lpGeometry.Dot(nvec, nvec2);
	const auto sin = lpGeometry.Cross(nvec, nvec2);
	dir_ = Vec2(cos, sin);
	fx_ = lpEffectMng.PickUp("fireBall", Vec2(0.0f,0.0f),pos_,Vec2(0.0f,0.0f), 19, 0.08f,flip,false,true);
	fx_.sprite_->setScale(0.6f);
	time_ = 0.0f;

	param.removeFlg = false;
	param.activation = true;
	param.name = "fireBall";
	param.ct = 30.0f * 60.0f;			// 30秒
}

SkillB::~SkillB()
{
}

SkillBase* SkillB::CreateSkillB(SkillBase* ptr)
{
	SkillBase* pRet = new(std::nothrow)SkillB(ptr);
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
	if (param.activation)
	{
		fx_.sprite_->setPosition(fx_.sprite_->getPosition() + (dir_.getNormalized() * 7.0f));
		effectData_.origin = fx_.sprite_->getPosition();
		// 絶対値をとり、敵に衝突もしくは時間経過で消えるようにしておく
		if ((abs(tpos_.x - fx_.sprite_->getPosition().x) <= 60.0f &&
			 abs(tpos_.y - fx_.sprite_->getPosition().y) <= 60.0f)||
			 time_ >= 19.0f * 0.08f)
		{
			param.activation = false;
		}
	}
}