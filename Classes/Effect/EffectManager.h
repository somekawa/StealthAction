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

	void update(float delta);

	void Play(const EffectType& eType, cocos2d::Vec2 pos);
	// �̪�Ă�۰��
	void Load(EffectType eType,int frame,float duration);
	// �̪�ı�Ұ��̷݂�����̎擾
	const cocos2d::Animation* GetAnimation(const EffectType& eType)
	{
		return effectAnimation_[eType];
	}

	const bool& GetAnimEnd(void)
	{
		return isAnimEnd_;
	}

private:
	float animFrame_;
	EffectType type_;
	bool isAnimEnd_;
	cocos2d::Animate* animate_;
	std::map<EffectType, cocos2d::Animation*> effectAnimation_;

	cocos2d::Vec2 pos_;
};

