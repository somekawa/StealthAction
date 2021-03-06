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
	// 更新
	void update(float delta);
	// Spriteのﾛｰﾄﾞ
	// 1回でいいのでこちらの関数を使用してﾛｰﾄﾞする
	static bool LoadSprite(void);
	// HPの生成
	static cocos2d::Sprite* CreateEnemyHPGauge(ActorType type,Enemy& target);
	bool Init(void);
	// HPのｾｯﾄ
	void SetHP(float hp);
	// HPの取得
	const float& GetHP(void) const
	{
		return currentHP_;
	}

private:
	// HPを付与するﾀｰｹﾞｯﾄの敵
	Enemy& target_;
	// 敵のﾀｲﾌﾟ
	ActorType enemyType_;
	// 敵の名前
	std::string enemyName_;
	// HPの現在の拡大率
	float curScale_;
	// HPの初期値
	float initHP_;
	// 現在のHP
	float currentHP_;
};

