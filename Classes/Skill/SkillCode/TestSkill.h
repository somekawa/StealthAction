#pragma once
#include "../SkillBase.h"
class TestSkill
	:public SkillBase
{
public:
	TestSkill(std::shared_ptr<SkillMng>skillMng);
	~TestSkill();
	void UpDate(void);
	void Init();
private:
protected:
};

