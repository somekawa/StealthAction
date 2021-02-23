#include <cmath>
#include "Generator/Geometry.h"
#include "Skill/SkillMng.h"
#include "SkillC.h"

USING_NS_CC;

SkillC::SkillC(SkillBase* ptr)
{
	pos_ = ptr->GetPlayerPos();
	effectData_.size = Size(120, 120);

	// ｴﾌｪｸﾄを作成して、自身のｴﾌｪｸﾄ画像に格納
	fx_ = lpEffectMng.PickUp("heal", Vec2{ 0.0f,0.0f }, Vec2(pos_.x,pos_.y + effectData_.size.height / 4), Vec2(0, 0), 9, 0.08f, false, false, true);

	// 加算合成(背景が黒のエフェクトの為、加算合成で透過が必要)
	BlendFunc func;
	func.src = (backend::BlendFactor)GL_SRC_ALPHA;
	func.dst = (backend::BlendFactor)GL_ONE;
	fx_.sprite_->setBlendFunc(func);

	param.removeFlg = false;
	param.activation = true;
	param.name = "heal";
	param.ct = 180.0f;
}

SkillC::~SkillC()
{
}

void SkillC::UpDate(float delta)
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

	test += delta;
	if (param.activation)
	{
		if (test >= 9 * 0.08f)	// アニメーション終了後
		{
			// SkillBaseのremoveFromParentの条件を満たすために切り替える
			param.activation = false;
		}
	}
}

void SkillC::Init()
{
}

bool SkillC::GetActive(void)
{
	return false;
}

cocos2d::Vec2 SkillC::GetPlayerPos(void)
{
	return cocos2d::Vec2();
}

cocos2d::Vec2 SkillC::GetTargetPos(void)
{
	return cocos2d::Vec2();
}

cocos2d::Rect SkillC::GetEffectData(void)
{
	return effectData_;
}

SkillBase* SkillC::CreateSkillC(SkillBase* ptr)
{
	SkillBase* pRet = new(std::nothrow)SkillC(ptr);
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
