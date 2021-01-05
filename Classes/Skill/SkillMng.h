#pragma once
#include <map>
#include <string>
struct SkillParamMng
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
	SkillParamMng GetSkillData(std::string name);
	//�S�f�[�^�̕ύX
	SkillParamMng ChangeAllData(std::string name, SkillParamMng param);
	//skill�̊J��
	bool SkillUnlock(std::string name);
	//skill�̋N��
	bool SkillActivate(std::string name);
	//skill�̒�~
	bool SkillInActivate(std::string name);

	void UpDate(void);
private:
	//�X�L���f�[�^�̕ۑ�
	std::map<std::string, SkillParamMng> skillData_;
};

