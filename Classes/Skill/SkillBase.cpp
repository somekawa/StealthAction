#include "Skill/SkillMng.h"
#include "SkillBase.h"

SkillBase::SkillBase()
{
	skillMng_ = std::make_shared<SkillMng>();
}

SkillBase::~SkillBase()
{
}

void SkillBase::AddActiveSkill(std::shared_ptr<SkillBase> ptr)
{
	skillList_.push_back(ptr);
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

void SkillBase::UpDate(std::string name)
{
	for (auto data : skillList_)
	{
		data->UpDate(name);
	}
}