// ’S“–êŠ
#pragma once
#include "../SkillBase.h"

class SkillB : public SkillBase
{
public:
	SkillB(SkillBase* ptr);
	~SkillB();
	void UpDate(float delta);
	static SkillBase* CreateSkillB(SkillBase* ptr);
private:
	cocos2d::Vec2 dir_;
	float time_;
protected:
};