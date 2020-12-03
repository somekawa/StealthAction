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
class Gravity;

#define FrameConverter(X) (X / 1000)

class Actor :
	public cocos2d::Sprite
{
public:
	// デフォルトコンストラクタ追加中
	//Actor();
	Actor();
	~Actor();
	// 各Actorの行動処理
	virtual void Action(void) = 0;
	// 更新処理
	virtual void Update(void);
	virtual void update(float delta) = 0;

	// 現在のアニメーションを取得
	virtual std::string GetAction(void);
	virtual void SetAction(std::string actName);

	// 方向変更
	virtual void ChangeDirection(void) = 0;
	// アニメーション登録
	virtual void AnimRegistrator(void) = 0;
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

	virtual const Vector2I& GetPos(void) const
	{
		return pos_;
	}

	virtual void SetPos(Vector2I pos);
	// 向いている方向のセット
	virtual void SetDirection(Direction dir);

	// これ作る必要あるのか分からないが、皆が使うと思い、作成しました
	// 整数値で現在のフレーム値を取得
	virtual const int& GetAnimationFrameInt(void);
	virtual bool CheckObjHit(void);

	virtual bool CheckOnFloor(void);
private:

protected:
	// ポジション
	Vector2I pos_;
	// 自身のサイズ
	Vector2I size_;
	// スピード
	Vector2I speed_;
	// キャラクターのタイプ
	ActorType type_;
	// 現在のｱﾆﾒｰｼｮﾝ
	std::string currentAnimation_;
	// 1フレーム前のアニメーション
	std::string previousAnimation_;
	// ｱﾆﾒｰｼｮﾝしている時はこのﾌﾚｰﾑ変数を加算
	float animationFrame_;
	// コライダーボックスの矩形表示や、データを配列から取り出すため、
	// 上記のアニメーションフレームをint型整数に直したframe値
	int animationFrame_int_;
	// アニメーションが終了しているかどうかのフラグ
	bool isAnimEnd_;

	// 画像反転のフラグ
	cocos2d::FlipX* flipFlag_;

	// 向いている方向
	Direction direction_;

	// オブジェクトを消すフラグ
	bool deleteFlag_;
	// 接地しているかのフラグ
	bool onFloor_;
	//std::unordered_map<std::string,
	//	std::vector<std::vector<std::shared_ptr<ActionRect>>>>&collider_;

	std::unordered_map<std::string,
		std::vector<std::vector<std::shared_ptr<ActionRect>>>>collider_;
	// 重力
	std::unique_ptr<Gravity> gravity_;

};