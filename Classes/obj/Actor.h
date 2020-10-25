#pragma once

#include <cocos2d.h> 
#include <memory>
#include <list>
#include <unordered_map>
#include "Geometory.h"
#include "Direction/Direction.h"
#include "ActorType.h"

class Input;
class Collider;
class ActionRect;

class Actor :
	public cocos2d::Sprite
{
public:
	// デフォルトコンストラクタ追加中
	Actor();
	Actor(std::unordered_map<std::string,
		std::vector<std::vector<std::shared_ptr<ActionRect>>>>&collider);
	~Actor();
	// 各Actorの行動処理
	virtual void Action(void) = 0;
	// 更新処理
	virtual void Update(void);
	virtual void update(float delta) = 0;

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

	virtual const Vector2F& GetPos(void) const
	{
		return pos_;
	}

private:

protected:
	// ポジション
	Vector2F pos_;
	// 自身のサイズ
	Vector2I size_;
	// スピード
	Vector2I speed_;
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

	// オブジェクトを消すフラグ
	bool deleteFlag_;
	//std::unordered_map<std::string,
	//	std::vector<std::vector<std::shared_ptr<ActionRect>>>>&collider_;

	std::unordered_map<std::string,
		std::vector<std::vector<std::shared_ptr<ActionRect>>>>collider_;

};