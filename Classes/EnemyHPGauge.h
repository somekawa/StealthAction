#pragma once
#include <cocos2d.h>
#include <string>
#include "obj/ActorType.h"

class Enemy;

class EnemyHPGauge :
	public cocos2d::Sprite
{
public:
	EnemyHPGauge(ActorType type,Enemy& target);
	~EnemyHPGauge();
	// �X�V
	void update(float delta);
	// Sprite��۰��
	// 1��ł����̂ł�����̊֐����g�p����۰�ނ���
	static bool LoadSprite(void);
	static cocos2d::Sprite* CreateEnemyHPGauge(ActorType type,Enemy& target);
	bool Init(void);
	// HP�̾��
	void SetHP(float hp);

	const float& GetHP(void) const
	{
		return currentHP_;
	}

private:
	Enemy& target_;
	// �G������
	ActorType enemyType_;
	// �G�̖��O
	std::string enemyName_;
	// HP�̌��݂̊g�嗦
	float curScale_;
	// HP�̏����l
	float initHP_;
	// ���݂�HP
	float currentHP_;
};

