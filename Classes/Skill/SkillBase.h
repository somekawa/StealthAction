// �ꕔ�S���ꏊ
#pragma once
#include <cocos2d.h>
#include <mutex>
#include <vector>
#include <string>
#include "Effect/EffectManager.h"
#include "Direction/Direction.h"

struct SkillParam
{
	std::string name;	// �X�L�����̕ۑ�
	bool lock;			// true: lock | false: unlock
	bool activation;	// ���ݔ������Ă��邩 true: ������ | false: ������
	bool removeFlg;		// �폜���Ă��悢�X�L���� true: �폜OK | false: �폜NG
	float ct;			// �N�[���^�C��
};

class SkillBase :public cocos2d::Sprite
{
public:
	static cocos2d::Sprite* createSkillBase();
	~SkillBase();
	void AddActiveSkill(SkillBase* ptr);
	void RemoveActiveSkill(void);
	virtual void UpDate(float delta);
	virtual void Render(void);
	std::vector<SkillBase*> GetSkillList(void);
	virtual cocos2d::Vec2 GetPlayerPos(void);				// �v���C���[���W
	virtual cocos2d::Vec2 GetTargetPos(void);				// �G���W(�z�[�~���O�n�̎��Ƀv���C���[�Ƃ̋�������ԋ߂��G�̍��W���擾������)
	virtual cocos2d::Rect GetEffectData(void) { return effectData_; };			// effectData�̎擾
	virtual const Direction& GetPlayerDirection(void);		// �v���C���[�̌���
	void SetPlayerPos(cocos2d::Vec2 playerPos);
	void SetTargetPos(cocos2d::Vec2 targetPos);
	void SetPlayerDirection(const Direction& direction);

	bool GetAllSkillActivate(void);				// �X�L���̂����ǂꂩ��active�ɂȂ��Ă�����true��Ԃ�
	bool GetSkillCT(std::string name);			// false�Ȃ�CT��0����Ȃ����߁A�܂����̃X�L���͎g�p�s��
	int GetSkillCTTime(std::string name);		// �X�L��CT�擾

	// �X�L���G�t�F�N�g���擾
	FXStruct& GetFX(void)
	{
		return fx_;
	}
private:
	CREATE_FUNC(SkillBase);
	cocos2d::Vec2 playerPos_;
	cocos2d::Vec2 targetPos_;
	Direction playerDirection_;
protected:
	cocos2d::Vec2 pos_;
	cocos2d::Vec2 tpos_;
	static std::vector<SkillBase*> skillList_;
	SkillParam param;

	FXStruct fx_;
	cocos2d::Rect effectData_;
};