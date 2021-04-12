#pragma once
#include <cocos2d.h> 
#include <memory>
#include <list>
#include <vector>
#include <string>
#include "Geometory.h"
#include "Direction/Direction.h"
#include "ActorType.h"
#include "anim/ActionCtl.h"
#include "input/OPRT_state.h"
#include "module/ActModule.h"
#include "../Loader/FileLoder.h"

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
	Actor(int hp,cocos2d::Layer& myLayer);
	~Actor();
	// 更新処理
	virtual void Update(void);
	// 更新
	virtual void update(float delta) = 0;
	// アニメーションの更新
	virtual void UpdateAnimation(float delta);
	// アニメーションの変更
	// param@ animName: 変更先アニメーション名
	virtual void ChangeAnimation(std::string animName);
	// 現在のアニメーションを取得
	virtual std::string GetAction(void);
	// currentAnimationのセット
	virtual void SetAction(std::string actName);
	// アニメーション登録
	virtual void AnimRegistrator(void);
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
	// ポジション取得
	virtual const Vector2I& GetPos(void) const
	{
		return pos_;
	}
	// 攻撃矩形の取得
	virtual const AttackRect& GetAttackRect(void)
	{
		return attackRect_;
	}
	// アニメーションのフレーム値の取得
	virtual const float& GetAnimationFrame(void)
	{
		return animationFrame_;
	}
	// ポジションセット
	virtual void SetPos(Vector2I pos);
	// 方向の取得
	virtual const Direction& GetDirection(void)
	{
		return direction_;
	}
	// 向いている方向のセット
	virtual void SetDirection(Direction dir);
	// 整数値で現在のフレーム値を取得
	virtual const int GetAnimationFrameInt(void);
	virtual const int GetAnimationFrameInt(std::string str);
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
	virtual void OnIsHitAttack(void)
	{
		isHitAttack_ = true;
	}
	void SetIsAttacking(bool flg);
	// 攻撃しているかのフラグ取得関数
	virtual const bool& IsAttacking(void)
	{
		return isAttacking_;
	}
	// 攻撃矩形のオフセットを設定
	virtual void SetAttackOffset(cocos2d::Vec2 offset);
	// 現在のコライダーボックスの取得
	const std::vector <std::shared_ptr<ActionRect>>& GetCurrentCol(void)
	{
		return currentCol_;
	}
	// HPの取得
	const int& GetHp(void)
	{
		return hp_;
	}
	// 自分のタイプ取得
	virtual const ActorType& GetType(void)
	{
		return type_;
	}
	// 攻撃オブジェクトを発射しているかの取得
	virtual bool GetFire(void)
	{
		return isFire_;
	}
	// 生きているかのフラグ取得
	virtual bool AliveCheck(void)
	{
		return isAlive_;
	}
	// モジュールの登録
	virtual  void ActModuleRegistration(void) = 0;							

	// 攻撃矩形の取得
	const cocos2d::Rect& GetAttackCol(void)
	{
		return attackCol_;
	}
	// ﾀﾞﾒｰｼﾞ矩形の取得
	const cocos2d::Rect& GetDamageCol(void)
	{
		return damageCol_;
	}
	// 各コライダー情報の設定
	virtual void SetCollider(void);

	// 当たり判定
	virtual bool OnHit(const cocos2d::Rect& collision);
	virtual void SetIsHitAttack(bool flg);
private:
	std::vector<std::string> split(std::string str, std::string separator);	// xmlデータをカンマで区切る
protected:
	void XmlActDataRead(std::string string, OPRT_state* oprt);		// アクションデータ読み込み
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
	// 現在のアニメーション
	std::string currentAnimation_;
	// 1フレーム前のアニメーション
	std::string previousAnimation_;
	// アニメーションしている時はこのフレーム変数を加算
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
	// 自分のコライダーデータ
	std::unordered_map<std::string,
		std::vector<std::vector<std::shared_ptr<ActionRect>>>>collider_;
	// 現在のコライダー
	std::vector <std::shared_ptr<ActionRect>> currentCol_;
	// 重力
	std::unique_ptr<Gravity> gravity_;
	// ダメージを食らったフラグ
	//bool onDamaged_;
	// 攻撃しているかのフラグ
	bool isAttacking_;
	// 攻撃に当たったかの判定を取る
	bool isHitAttack_;
	// 攻撃矩形
	AttackRect attackRect_;
	// 自分自身が存在するレイヤー
	cocos2d::Layer& myLayer_;

	cocos2d::Layer* attackLayer_;
	// 攻撃オブジェクトを出しているかのフラグ
	bool isFire_;
	// 生きているか死んでいるかのフラグ
	bool isAlive_;

	// ActionCtlを呼び出す
	ActionCtl actCtl_;		

	//読み込んだファイルデータの保存
	DataTable fileLoad_;

	// 攻撃矩形の格納先
	// first : 矩形データ
	// second : 攻撃矩形の名前
	cocos2d::Rect attackCol_;
	// ダメージ矩形の格納先
	cocos2d::Rect damageCol_;
};