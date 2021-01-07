#pragma once
#include <cocos2d.h>
#include <string>
#include "Geometory.h"
#include "../ActorType.h"

class Actor;

class Obj :
	public cocos2d::Sprite
{
public:
	Obj(Actor& target);
	~Obj();
	// 更新
	void update(float delta) = 0;
	// 動き
	virtual void Move(void) = 0;
	// 各ｱﾆﾒｰｼｮﾝの登録
	virtual void AnimRegistrator(void) = 0;
	// ｱﾆﾒｰｼｮﾝの初期化
	virtual bool InitAnim(void);
	// 飛ばした対象に当たったかどうかのﾁｪｯｸ関数
	// 当たっていればtrueいなければfalse
	virtual bool HitToTarget(void);

	// アニメーションの更新
	virtual void UpdateAnimation(float delta);
	// アニメーションの変更
	// param@ animName: 変更先アニメーション名
	virtual void ChangeAnimation(std::string animName);
private:

protected:
	// ｱﾆﾒｰｼｮﾝｺﾏ数ｶｳﾝﾄ変数
	float animationFrame_;
	// ｱﾆﾒｰｼｮﾝが終了したかのﾌﾗｸﾞ
	// 終了している:true
	// 終了していない:false
	bool isAnimEnd_;
	// 自身のﾀｲﾌﾟ
	ActorType type_;
	std::string currentAnimation_;
	// 左右反転ﾌﾗｸﾞ
	cocos2d::FlipX* flipFlag_;
	// 今現在狙っている対象
	Actor& target_;
	// ﾀｰｹﾞｯﾄと当たっているかのﾌﾗｸﾞ
	bool isHitTarget_;
};

