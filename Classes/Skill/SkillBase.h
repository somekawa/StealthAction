#pragma once
#include <cocos2d.h>
#include <mutex>
#include <vector>
#include <string>

struct SkillParam
{
	std::string name;	//スキル名の保存
	bool lock;	//true: lock | false: unlock
	bool activation;	//現在発動しているか true: 発動中 | false: 未発動
	float ct;	//クールタイム
};

//class SkillMng;
class SkillBase :public cocos2d::Sprite
{
public:
	static cocos2d::Sprite* createSkillBase();
	//SkillBase();
	~SkillBase();
	void AddActiveSkill(std::shared_ptr<SkillBase> ptr);
	void RemoveActiveSkill(void);
	virtual void UpDate(void);
private:
	CREATE_FUNC(SkillBase);
	//std::shared_ptr<SkillMng> skillMng_;
protected:
	//virtual bool GetActive(void) = 0;
	
	std::vector<std::shared_ptr<SkillBase>> skillList_;
	SkillParam param;
};

