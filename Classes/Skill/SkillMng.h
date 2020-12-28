#pragma once
#include <map>
struct SkillParam
{
	std::string name;	//�X�L�����̕ۑ�
	bool lock;	//true: lock | false: unlock
	bool activation;	//���ݔ������Ă��邩 true: ������ | false: ������
	float ct;	//�N�[���^�C��
};

class SkillMng
{
public:
	SkillMng();
	~SkillMng();

	//skill�f�[�^�̎擾
	SkillParam GetSkillData(std::string name);
	//�S�f�[�^�̕ύX
	SkillParam ChangeAllData(std::string name, SkillParam param);
	//skill�̊J��
	bool SkillUnlock(std::string name);
	//skill�̋N��
	bool SkillActivate(std::string name);
	//skill�̒�~
	bool SkillInActivate(std::string name);

	void UpDate(std::string name);
private:
	//�X�L���f�[�^�̕ۑ�
	std::map<std::string, SkillParam> skillData_;
};

