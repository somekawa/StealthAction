#pragma once
#include "../SkillBase.h"
class SkillA : public SkillBase
{
public:
	SkillA(SkillBase* ptr);
	~SkillA();
	void UpDate(float delta);
	void Init();
	bool GetActive(void);
	cocos2d::Vec2 GetPlayerPos(void);
	cocos2d::Vec2 GetTargetPos(void);
	cocos2d::Rect GetEffectData(void);
	static SkillBase* CreateSkillA(SkillBase* ptr);
private:
protected:
};