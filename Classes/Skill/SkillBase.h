#pragma once
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

class  SkillMng;
class SkillBase
{
public:
	SkillBase();
	~SkillBase();
	void AddActiveSkill(std::shared_ptr<SkillBase> ptr);
	void RemoveActiveSkill(void);
	virtual void UpDate(std::string name);
private:
	std::shared_ptr<SkillMng> skillMng_;
protected:
	virtual bool GetActive(void) = 0;
	
	std::vector<std::shared_ptr<SkillBase>> skillList_;
	SkillParam param;
};

