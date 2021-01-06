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
	return pRet;
}

SkillBase::~SkillBase()
{
}

void SkillBase::AddActiveSkill(TestSkill* testskill_ptr)
{
	skillList_.push_back(testskill_ptr);
}

void SkillBase::RemoveActiveSkill(void)
{
	for (auto itr = skillList_.begin();itr != skillList_.end();)
	{
		if ((*itr)->param.activation == false)
		{
			skillList_.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void SkillBase::UpDate(void)
{
	for (auto data : skillList_)
	{
		data->UpDate();
	}
}