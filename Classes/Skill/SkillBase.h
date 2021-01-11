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
	void AddActiveSkill(SkillBase* ptr);
	void RemoveActiveSkill(void);
	virtual void UpDate(void);
	virtual cocos2d::Vec2 GetPlayerPos(void);	// �v���C���[���W
	virtual cocos2d::Vec2 GetTargetPos(void);	// �G���W(�z�[�~���O�n�̎��Ƀv���C���[�Ƃ̋�������ԋ߂��G�̍��W���擾������)
	void SetPlayerPos(cocos2d::Vec2 playerPos);
	void SetTargetPos(cocos2d::Vec2 targetPos);
private:
	CREATE_FUNC(SkillBase);
	cocos2d::Vec2 playerPos_;
	cocos2d::Vec2 targetPos_;
	//std::shared_ptr<SkillMng> skillMng_;
protected:
	//virtual bool GetActive(void) = 0;
	
	std::vector<SkillBase*> skillList_;
	SkillParam param;
};

