#pragma once
#include <cocos2d.h>
#include <map>
#include <string>
#include <list>

// ｴﾌｪｸﾄの再生ﾀｲﾌﾟ
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
	// 更新
	void update(float delta);
	// ｴﾌｪｸﾄの再生
	void Play(const EffectType& eType, cocos2d::Vec2 pos);
	// ｴﾌｪｸﾄのﾛｰﾄﾞ
	void Load(EffectType eType,int frame,float duration);
	// ｴﾌｪｸﾄｱﾆﾒｰｼｮﾝのｷｬｯｼｭの取得
	const cocos2d::Animation* GetAnimation(const EffectType& eType)
	{
		return effectAnimation_[eType];
	}
	// ｱﾆﾒｰｼｮﾝが終了したかのﾌﾗｸﾞ取得
	const bool& GetAnimEnd(void)
	{
		return isAnimEnd_;
	}

private:
	// ｱﾆﾒｰｼｮﾝのコマ数ｶｳﾝﾄ変数
	float animFrame_;
	// ｴﾌｪｸﾄのﾀｲﾌﾟ
	EffectType type_;
	// ｱﾆﾒｰｼｮﾝが終了したかのﾌﾗｸﾞ
	// 終了している:true
	// 終了していない:false
	bool isAnimEnd_;
	// 再生するｴﾌｪｸﾄのｱﾆﾒｰｼｮﾝﾃﾞｰﾀ
	cocos2d::Animate* animate_;
	// ｴﾌｪｸﾄ毎のｱﾆﾒｰｼｮﾝﾃﾞｰﾀ
	std::map<EffectType, cocos2d::Animation*> effectAnimation_;
	// ｴﾌｪｸﾄを再生するﾎﾟｼﾞｼｮﾝ
	cocos2d::Vec2 pos_;
};

