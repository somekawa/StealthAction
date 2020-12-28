#pragma once
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

