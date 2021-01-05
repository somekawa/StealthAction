#include <vector>
#include <string>
#include "SkillMng.h"

SkillMng::SkillMng(void)
{
}

SkillMng::~SkillMng()
{
}

SkillParamMng SkillMng::GetSkillData(std::string name)
{
	return skillData_[name];
}

// fileloaderで読み込んで初期化に使う
SkillParamMng SkillMng::ChangeAllData(std::string name, SkillParamMng param)
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

void SkillMng::UpDate(void)
{
	for (auto data : skillData_)
	{
		if (data.second.lock == true)
		{
			return;
		}
		if (data.second.activation == true)
		{
			if (data.second.ct > 0.0f)
			{
				data.second.ct--;
			}
			else
			{
				//ここでCTをリセットする
				data.second.ct;
				SkillInActivate(data.second.name);
			}
		}
	}
}
