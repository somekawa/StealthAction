#pragma once
#include <cocos2d.h>
#include <map>
#include <string>
#include <list>

// �̪�Ă̍Đ�����
enum class EffectType
{
	PlayerSpawn,
	EnemySpawn,
	PlayerAttack1,
	PlayerAttack2,
	Max
};

class EffectManager:
	public cocos2d::Sprite
{
public:
	EffectManager(cocos2d::Layer& layer);
	~EffectManager();
	// �X�V
	void update(float delta);
	// �̪�Ă̍Đ�
	void Play(const EffectType& eType, cocos2d::Vec2 pos);
	// �̪�Ă�۰��
	void Load(EffectType eType,int frame,float duration);
	// �̪�ı�Ұ��̷݂�����̎擾
	const cocos2d::Animation* GetAnimation(const EffectType& eType)
	{
		return effectAnimation_[eType];
	}
	// ��Ұ��݂��I�����������׸ގ擾
	const bool& GetAnimEnd(void)
	{
		return isAnimEnd_;
	}

private:
	// ��Ұ��݂̃R�}�����ĕϐ�
	float animFrame_;
	// �̪�Ă�����
	EffectType type_;
	// ��Ұ��݂��I�����������׸�
	// �I�����Ă���:true
	// �I�����Ă��Ȃ�:false
	bool isAnimEnd_;
	// �Đ�����̪�Ă̱�Ұ����ް�
	cocos2d::Animate* animate_;
	// �̪�Ė��̱�Ұ����ް�
	std::map<EffectType, cocos2d::Animation*> effectAnimation_;
	// �̪�Ă��Đ������߼޼��
	cocos2d::Vec2 pos_;
};

