#include <Windows.h>
#include <vector>
#include <string>
#include "SkillMng.h"

SkillMng::SkillMng(std::string directory)
{
	HANDLE h_find;
	WIN32_FIND_DATAA win32_fd;
	std::string Search_name = "Skill/skill_data/"+ directory +"/*.png";
	std::vector<std::string> f_names;
	h_find = FindFirstFileA(Search_name.c_str(), &win32_fd);
	do
	{
		/*取得したパスがディレクトリであるかどうかの判定*/
		if (win32_fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
		}
		else
		{
			std::string f_name = win32_fd.cFileName;
			f_name = f_name.erase(f_name.find("."));
			skill_data_[f_name].lock = true;
			f_names.emplace_back(f_name);
		}
	} while (FindNextFileA(h_find, &win32_fd));

	FindClose(h_find);
}

SkillMng::~SkillMng()
{
}

SkillParam SkillMng::GetSkillData(std::string name)
{
	return skill_data_[name];
}

SkillParam SkillMng::ChangeAllData(std::string name, SkillParam param)
{
	return skill_data_[name] = param;
}

bool SkillMng::SkillUnlock(std::string name)
{
	return skill_data_[name].lock = false;
}

bool SkillMng::SkillInActivate(std::string name)
{
	return skill_data_[name].activation = false;
}

bool SkillMng::SkillActivate(std::string name)
{
	return skill_data_[name].activation = true;
}

void SkillMng::UpDate(std::string name)
{
	if (skill_data_[name].lock == true)
	{
		return;
	}
	if(skill_data_[name].activation == true)
	{
		if (skill_data_[name].ct > 0.0f)
		{
			skill_data_[name].ct--;
		}
		else
		{
			//ここでCTをリセットする
			skill_data_[name].ct;
			SkillInActivate(name);
		}
	}
}
