#pragma once

#include <cocos2d.h> 
#include <memory>
#include <list>
#include "Geometory.h"
#include "Direction/Direction.h"
#include "ActorType.h"
#include "anim/ActionCtl.h"
#include "input/OPRT_state.h"
#include "module/ActModule.h"

class Input;
class Collider;
class ActionRect;
class Gravity;

// 攻撃の矩形
struct AttackRect
{
	bool offsetFlag_ = false;
	// ポジション
	cocos2d::Vec2 offset_ = {0.0f,0.0f};
	// サイズ
	cocos2d::Size size_;
};

#define FrameConverter(X) (X / 1000)

class Actor :
	public cocos2d::Sprite
{
public:
	// デフォルトコンストラクタ追加中
	//Actor();
	Actor(int hp,cocos2d::Layer& myLayer);
	~Actor();
	// 各Actorの行動処理
	virtual void Action(void) = 0;
	// 更新処理
	virtual void Update(void);
	// 更新
	virtual void update(float delta) = 0;

	// 現在のアニメーションを取得
	virtual std::string GetAction(void);
	// currentAnimationのｾｯﾄ
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
	// ﾎﾟｼﾞｼｮﾝ取得
	virtual const Vector2I& GetPos(void) const
	{
		return pos_;
	}

	// 攻撃矩形の取得
	virtual const AttackRect& GetAttackRect(void)
	{
		return attackRect_;
	}
	// ｱﾆﾒｰｼｮﾝのﾌﾚｰﾑ値の取得
	virtual const float& GetAnimationFrame(void)
	{
		return animationFrame_;
	}
	// ﾎﾟｼﾞｼｮﾝｾｯﾄ
	virtual void SetPos(Vector2I pos);
	// 方向の取得
	virtual const Direction& GetDirection(void)
	{
		return direction_;
	}
	// 向いている方向のセット
	virtual void SetDirection(Direction dir);

	// これ作る必要あるのか分からないが、皆が使うと思い、作成しました
	// 整数値で現在のフレーム値を取得
	virtual const int& GetAnimationFrameInt(void);

	virtual void CheckMapObjHit(float delta);
	// 壁に当たっているか判定
	virtual const bool& IsHitWall(void)
	{
		return isHitWall_;
	}
	// 床に接地しているかの判定
	virtual const bool& OnFloor(void)
	{
		return onFloor_;
	}
	// ダメージをくらったときに使用
	virtual void OnDamaged(void)
	{
		onDamaged_ = true;
	}
	void SetIsAttacking(bool flg);
	// 攻撃しているかのフラグ取得関数
	virtual const bool& IsAttacking(void)
	{
		return isAttacking_;
	}
	// ﾀﾞﾒｰｼﾞをくらったﾌﾗｸﾞの取得関数
	virtual const bool& GetOnDamaged(void)
	{
		return onDamaged_;
	}
	// 攻撃矩形のｵﾌｾｯﾄを設定
	virtual void SetAttackOffset(cocos2d::Vec2 offset);
	// 現在のコライダーボックスの取得
	const std::vector <std::shared_ptr<ActionRect>>& GetCurrectCol(void)
	{
		return currentCol_;
	}
	// HPの取得
	const int& GetHp(void)
	{
		return hp_;
	}
	// 自分のﾀｲﾌﾟ取得
	virtual const ActorType& GetType(void)
	{
		return type_;
	}
	// 攻撃ｵﾌﾞｼﾞｪｸﾄを発射しているかの取得
	virtual bool GetFire(void)
	{
		return isFire_;
	}
	// 生きているかのﾌﾗｸﾞ取得
	virtual bool AliveCheck(void)
	{
		return isAlive_;
	}
	// モジュールの登録
	virtual  void actModuleRegistration(void) = 0;							

private:

protected:
	// hp
	int hp_;
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
	// 壁に当たったかのフラグ
	bool isHitWall_;
	// 自分のｺﾗｲﾀﾞｰﾃﾞｰﾀ
	std::unordered_map<std::string,
		std::vector<std::vector<std::shared_ptr<ActionRect>>>>collider_;
	// 現在のコライダー
	std::vector <std::shared_ptr<ActionRect>> currentCol_;
	// 重力
	std::unique_ptr<Gravity> gravity_;
	// ダメージを食らったフラグ
	bool onDamaged_;
	// 攻撃しているかのフラグ
	bool isAttacking_;
	// 攻撃矩形
	AttackRect attackRect_;
	// 自分自身が存在するﾚｲﾔｰ
	cocos2d::Layer& myLayer_;

	cocos2d::Layer* attackLayer_;
	// 攻撃ｵﾌﾞｼﾞｪｸﾄを出しているかのﾌﾗｸﾞ
	bool isFire_;
	// 生きているか死んでいるかのﾌﾗｸﾞ
	bool isAlive_;

	// ActionCtlを呼び出す
	ActionCtl actCtl_ = ActionCtl();							
};