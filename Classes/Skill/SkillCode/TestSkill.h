#pragma once
#include "../SkillBase.h"
class TestSkill : public SkillBase
{
public:
	TestSkill();
	~TestSkill();
	void UpDate(void);
	void Init();
	bool GetActive(void);
private:
protected:
};

