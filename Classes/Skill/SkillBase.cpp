//#include "SkillMng.h"
#include "SkillBase.h"
#include "SkillCode/TestSkill.h"

//SkillBase::SkillBase()
//{
//	skillMng_ = std::make_shared<SkillMng>();
//}

cocos2d::Sprite* SkillBase::createSkillBase()
{
	SkillBase* pRet = new(std::nothrow)SkillBase();
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

SkillBase::~SkillBase()
{
}

void SkillBase::AddActiveSkill(SkillBase* ptr)
{
	skillList_.push_back(ptr);
}

void SkillBase::RemoveActiveSkill(void)
{
	for (auto itr = skillList_.begin();itr != skillList_.end();)
	{
		if ((*itr)->param.activation == false)
		{
			(*itr)->removeFromParent();
			itr = skillList_.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void SkillBase::UpDate(float delta)
{
	for (auto data : skillList_)
	{
		data->UpDate(delta);
	}
	RemoveActiveSkill();
}

void SkillBase::Render(void)
{
}

cocos2d::Vec2 SkillBase::GetPlayerPos(void)
{
	return playerPos_;
}

const Direction& SkillBase::GetPlayerDirection(void)
{
	return playerDirection_;
}

cocos2d::Vec2 SkillBase::GetTargetPos(void)
{
	return targetPos_;
}

void SkillBase::SetPlayerPos(cocos2d::Vec2 playerPos)
{
	playerPos_ = playerPos;
}

void SkillBase::SetTargetPos(cocos2d::Vec2 targetPos)
{
	targetPos_ = targetPos;
}

void SkillBase::SetPlayerDirection(const Direction& direction)
{
	playerDirection_ = direction;
}
