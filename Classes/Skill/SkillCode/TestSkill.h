#pragma once
#include "../SkillBase.h"
class TestSkill : public SkillBase
{
public:
	TestSkill(SkillBase* ptr);
	~TestSkill();
	void UpDate(float delta);
	void Init();
	bool GetActive(void);
	cocos2d::Vec2 GetPlayerPos(void);
	cocos2d::Vec2 GetTargetPos(void);
	cocos2d::Rect GetEffectData(void);
private:
protected:
};

