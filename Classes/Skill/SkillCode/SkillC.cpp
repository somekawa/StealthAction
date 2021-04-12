// 担当場所
#include <cmath>
#include "Generator/Geometry.h"
#include "Skill/SkillMng.h"
#include "SkillC.h"

USING_NS_CC;

SkillC::SkillC(SkillBase* ptr)
{
	pos_ = ptr->GetPlayerPos();
	effectData_.size = Size(120.0f, 120.0f);

	fx_ = lpEffectMng.PickUp("heal", Vec2(0.0f,0.0f), Vec2(pos_.x,pos_.y + effectData_.size.height / 4.0f), Vec2(0.0f, 0.0f), 9, 0.08f, false, false, true);

	// 加算合成(背景が黒のエフェクトの為、加算合成で透過が必要)
	BlendFunc func;
	func.src = (backend::BlendFactor)GL_SRC_ALPHA;
	func.dst = (backend::BlendFactor)GL_ONE;
	fx_.sprite_->setBlendFunc(func);

	param.removeFlg  = false;
	param.activation = true;
	param.name = "heal";
	param.ct = 35.0f * 60.0f;		// 35秒

	time_ = 0.0f;
}

SkillC::~SkillC()
{
}

SkillBase* SkillC::CreateSkillC(SkillBase* ptr)
{
	SkillBase* pRet = new(std::nothrow)SkillC(ptr);
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

void SkillC::UpDate(float delta)
{
	if (!param.activation)
	{
		if (param.ct > 0.0f)
		{
			param.ct--;
		}
		else
		{
			param.ct = 0.0f;
			param.removeFlg = true;
		}
	}

	time_ += delta;
	if (param.activation)
	{
		if (time_ >= 9.0f * 0.08f)	// アニメーション終了後
		{
			param.activation = false;
		}
	}
}