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
	// XV
	void update(float delta);
	// Sprite‚ÌÛ°ÄŞ
	// 1‰ñ‚Å‚¢‚¢‚Ì‚Å‚±‚¿‚ç‚ÌŠÖ”‚ğg—p‚µ‚ÄÛ°ÄŞ‚·‚é
	static bool LoadSprite(void);
	// HP‚Ì¶¬
	static cocos2d::Sprite* CreateEnemyHPGauge(ActorType type,Enemy& target);
	bool Init(void);
	// HP‚Ì¾¯Ä
	void SetHP(float hp);
	// HP‚Ìæ“¾
	const float& GetHP(void) const
	{
		return currentHP_;
	}

private:
	// HP‚ğ•t—^‚·‚éÀ°¹Ş¯Ä‚Ì“G
	Enemy& target_;
	// “G‚ÌÀ²Ìß
	ActorType enemyType_;
	// “G‚Ì–¼‘O
	std::string enemyName_;
	// HP‚ÌŒ»İ‚ÌŠg‘å—¦
	float curScale_;
	// HP‚Ì‰Šú’l
	float initHP_;
	// Œ»İ‚ÌHP
	float currentHP_;
};

