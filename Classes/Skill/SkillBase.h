#pragma once
#include <cocos2d.h>
#include <mutex>
#include <vector>
#include <string>

#include "Effect/EffectManager.h"
#include "Direction/Direction.h"

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
	virtual void UpDate(float delta);
	virtual void Render(void);
	virtual cocos2d::Vec2 GetPlayerPos(void);	// �v���C���[���W
	virtual const Direction& GetPlayerDirection(void); // ��ڲ԰�̌���
	virtual cocos2d::Vec2 GetTargetPos(void);	// �G���W(�z�[�~���O�n�̎��Ƀv���C���[�Ƃ̋�������ԋ߂��G�̍��W���擾������)
	void SetPlayerPos(cocos2d::Vec2 playerPos);
	void SetTargetPos(cocos2d::Vec2 targetPos);
	void SetPlayerDirection(const Direction& direction);
	// ��ٴ̪�ď��擾
	FXStruct& GetFX(void)
	{
		return fx_;
	}
private:
	CREATE_FUNC(SkillBase);
	cocos2d::Vec2 playerPos_;
	cocos2d::Vec2 targetPos_;
	Direction playerDirection_;
	//std::shared_ptr<SkillMng> skillMng_;
protected:
	//virtual bool GetActive(void) = 0;
	cocos2d::Vec2 pos_;
	cocos2d::Vec2 tpos_;
	std::vector<SkillBase*> skillList_;
	SkillParam param;
	// ���g�̴̪�Ă̽��ײ�
	cocos2d::Sprite* effectSprite_;

	FXStruct fx_;
};

