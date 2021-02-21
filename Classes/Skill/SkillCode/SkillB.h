#pragma once
#include "../SkillBase.h"
class SkillB : public SkillBase
{
public:
	SkillB(SkillBase* ptr);
	~SkillB();
	void UpDate(float delta);
	void Init();
	bool GetActive(void);
	cocos2d::Vec2 GetPlayerPos(void);
	cocos2d::Vec2 GetTargetPos(void);
	cocos2d::Rect GetEffectData(void);
	static SkillBase* CreateSkillB(SkillBase* ptr);
private:
	cocos2d::Vec2 dir_;
	float time_;
protected:
};

