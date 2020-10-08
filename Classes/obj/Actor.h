#pragma once

#include <cocos2d.h> 
#include <memory>
#include <list>

#include "Direction/Direction.h"

enum class ActorType
{
	Player,
	Imp,
	Max
};

class Input;
class Collider;

class Actor :
	public cocos2d::Sprite
{
public:
	Actor();
	~Actor();
	// 各Actorの行動処理
	virtual void Action(void) = 0;
	// 更新処理
	virtual void Update(void);

	// 自身のspriteを返す
	virtual const cocos2d::Sprite* GetSprite(void) const
	{
		return sprite_;
	}
	// 方向変更
	virtual void ChangeDirection(void) = 0;

	// オブジェクトが死んだらdeleteFlagをtrueにする
	virtual void Delete(void)
	{
		deleteFlag_ = true;
	}
	// deleteFlagがtrueになっているオブジェクトを見つけ、listから消去するために使用
	virtual const bool GetDeleteFlag(void)
	{
		return deleteFlag_;
	}

	const cocos2d::Vec2& GetPos(void) const
	{
		return pos_;
	}

private:
	// オブジェクトを消すフラグ
	bool deleteFlag_;
protected:
	// ポジション
	cocos2d::Vec2 pos_;
	// 自身のサイズ
	cocos2d::Vec2 size_;
	// スピード
	cocos2d::Vec2 speed_;
	// スプライト
	cocos2d::Sprite* sprite_;
	// キャラクターのタイプ
	ActorType type_;
	// 現在のｱﾆﾒｰｼｮﾝ
	std::string currentAnimation_;
	// ｱﾆﾒｰｼｮﾝしている時はこのﾌﾚｰﾑ変数を加算
	float animationFrame_;
	// 画像反転のフラグ
	cocos2d::FlipX* flipFlag_;

	// 向いている方向
	Direction direction_;

	//std::list<std::unique_ptr<Collider>> collider_;
	Collider* collider_;
};