#include <cmath>
#include "Generator/Geometry.h"
#include "Skill/SkillMng.h"
#include "SkillC.h"

USING_NS_CC;

SkillC::SkillC(SkillBase* ptr)
{
	pos_ = ptr->GetPlayerPos();
	effectData_.size = Size(120, 120);

	// ´Ìª¸Ä‚ðì¬‚µ‚ÄAŽ©g‚Ì´Ìª¸Ä‰æ‘œ‚ÉŠi”[
	fx_ = lpEffectMng.PickUp("heal", Vec2{ 0.0f,0.0f }, Vec2(pos_.x,pos_.y + effectData_.size.height / 4), Vec2(0, 0), 9, 0.08f, false, false, true);

	// ‰ÁŽZ‡¬(”wŒi‚ª•‚ÌƒGƒtƒFƒNƒg‚Ìˆ×A‰ÁŽZ‡¬‚Å“§‰ß‚ª•K—v)
	BlendFunc func;
	func.src = (backend::BlendFactor)GL_SRC_ALPHA;
	func.dst = (backend::BlendFactor)GL_ONE;
	fx_.sprite_->setBlendFunc(func);
}

SkillC::~SkillC()
{
}

void SkillC::UpDate(float delta)
{
	// SkillC‚ð”­“®‚³‚¹‚é‚Æ‚«‚ÉASkillA‚Ìupdate‚à‰ñ‚Á‚Ä‚¢‚é?
	if (fx_.isActive_)
	{
		int a = 0;
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
