// ’S“–êŠ
#pragma once
#include "../SkillBase.h"

class SkillA : public SkillBase
{
public:
	SkillA(SkillBase* ptr);
	~SkillA();
	void UpDate(float delta);
	static SkillBase* CreateSkillA(SkillBase* ptr);
private:
	float speed_;
protected:
};