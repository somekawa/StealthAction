// ’S“–êŠ
#pragma once
#include "../SkillBase.h"

class SkillC : public SkillBase
{
public:
	SkillC(SkillBase* ptr);
	~SkillC();
	void UpDate(float delta);
	static SkillBase* CreateSkillC(SkillBase* ptr);
private:
	float time_;
protected:
};