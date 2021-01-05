#pragma once
#include <cocos2d.h>
#include <mutex>
#include <vector>
#include <string>

struct SkillParam
{
	std::string name;	//�X�L�����̕ۑ�
	bool lock;	//true: lock | false: unlock
	bool activation;	//���ݔ������Ă��邩 true: ������ | false: ������
	float ct;	//�N�[���^�C��
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

