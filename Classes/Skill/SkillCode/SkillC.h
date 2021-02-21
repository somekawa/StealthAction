#pragma once
#include "../SkillBase.h"
class SkillC : public SkillBase
{
public:
	SkillC(SkillBase* ptr);
	~SkillC();
	void UpDate(float delta);
	void Init();
	bool GetActive(void);
	cocos2d::Vec2 GetPlayerPos(void);
	cocos2d::Vec2 GetTargetPos(void);
	cocos2d::Rect GetEffectData(void);
	static SkillBase* CreateSkillC(SkillBase* ptr);
private:
protected:
};