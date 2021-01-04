#include <vector>
#include <string>
#include "SkillMng.h"

SkillMng::SkillMng(void)
{
}

SkillMng::~SkillMng()
{
}

SkillParam SkillMng::GetSkillData(std::string name)
{
	return skillData_[name];
}

SkillParam SkillMng::ChangeAllData(std::string name, SkillParam param)
{
	return skillData_[name] = param;
}

bool SkillMng::SkillUnlock(std::string name)
{
	return skillData_[name].lock = false;
}

bool SkillMng::SkillInActivate(std::string name)
{
	return skillData_[name].activation = false;
}

bool SkillMng::SkillActivate(std::string name)
{
	return skillData_[name].activation = true;
}

void SkillMng::UpDate(std::string name)
{
	if (skillData_[name].lock == true)
	{
		return;
	}
	if(skillData_[name].activation == true)
	{
		if (skillData_[name].ct > 0.0f)
		{
			skillData_[name].ct--;
		}
		else
		{
			//‚±‚±‚ÅCT‚ðƒŠƒZƒbƒg‚·‚é
			skillData_[name].ct;
			SkillInActivate(name);
		}
	}
}
