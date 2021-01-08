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

class TestSkill;
// ����testSkill���Ă΂��ۂ�SkillBase���Ă΂��̂ŁA�����̺ݽ�׸��Ŗ��O�ް����i�[�������skillData�Ƃ��Ďg�p����B

class SkillBase :public cocos2d::Sprite
{
public:
	static cocos2d::Sprite* createSkillBase();
	//SkillBase();
	~SkillBase();
	void AddActiveSkill(TestSkill* testskill_ptr);
	void RemoveActiveSkill(void);
	virtual void UpDate(void);
private:
	CREATE_FUNC(SkillBase);
	//std::shared_ptr<SkillMng> skillMng_;
protected:
	//virtual bool GetActive(void) = 0;
	
	std::vector<TestSkill*> skillList_;
	SkillParam param;
};

