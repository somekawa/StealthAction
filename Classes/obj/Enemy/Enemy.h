#pragma once
#include <memory>
#include <list>
#include <array>
#include "obj/Actor.h"

class Player;
class BehaviorTree;
class BehaviorData;
class NodeBase;

// 行動タイプ
enum class MoveType
{
	// 何も動いていない
	Non,
	// 追跡
	Chase,
	// 巡回
	Patrol,
	Max
};

// 視界範囲
// 敵の種類によって視界範囲をそれぞれ持たせたいので
struct VisionRange
{
	// 攻撃範囲
	float attack_;
	// 追跡モードになる範囲
	float chase_;
	VisionRange():attack_(0.0f),chase_(0.0f) {};
	VisionRange(float attack, float chase) :attack_(attack), chase_(chase) {};
};

class Enemy :
	public Actor
{
public:
	Enemy(cocos2d::Vec2 pos,Player& player,
		BehaviorTree* aiTree,VisionRange visionRange,int hp,cocos2d::Layer& myLayer);
	~Enemy();

	virtual void Action(void) = 0;

	// 視界に入ったかの計算をし、vision_を返す
	virtual const float& GetVision(void);

	// 自分自身の名前の取得
	const std::string& GetMyName(void)
	{
		return myName_;
	}

	// 各視界範囲の取得
	virtual const VisionRange& GetVisionRange(void)
	{
		return visionRange_;
	}
	// 今現在のﾌﾛｱに存在する自分自身を消す(ﾌﾛｱ変更の際に使用)
	virtual void DeleteSelfOnFloor(void);

	virtual void AddAttackObj(const float& angle) = 0;

	virtual void ChangeDirection(void);
	virtual const float& DistanceCalcurator(void);
	// 生きているかのﾌﾗｸﾞのｾｯﾄ
	void SetAlive(bool flg);
	// 自分の行動タイプの取得
	virtual const MoveType& GetMoveType(void)
	{
		return mType_;
	}
	// 自身の固有IDのｾｯﾄ
	void SetID(int id);
	// 自身の固有IDの取得
	const int& GetID(void)
	{
		return id_;
	}

	virtual void update(float delta) = 0;
	// 自分の攻撃がプレイヤーに当たっているかの判定
	virtual bool OnAttacked(void);
	// ﾌﾟﾚｲﾔｰの当たり矩形との当たり判定
	virtual void CheckHitPLAttack(void);
	// アニメーションの更新
	virtual void UpdateAnimation(float delta);
	// アニメーションの変更
	// param@ animName: 変更先アニメーション名
	virtual void ChangeAnimation(std::string animName);

	virtual void AnimRegistrator(void) = 0;
	// モジュールの登録
	virtual void actModuleRegistration(void) = 0;							

	// AI行動のNodeを探索して実行
	virtual void AIRun(void);
	// 移動タイプのセット
	// 上記にenumあり
	// 行動タイプ
	//enum class MoveType
	//{
	//	// 何も動いていない
	//	Non,
	//	// 追跡
	//	Chase,
	//	// 巡回
	//	Patrol,
	//	Max
	//};
	virtual void SetMoveType(MoveType type);

	// 壁や床にぶつかっているかの判定
	virtual bool CheckObjHit(void);
	// Mapのオブジェクトに当たったかの関数
	//virtual void CheckMapObjHit(float delta);
	// 物理攻撃
	virtual void NormalAttack(void) = 0;
	// スキル使用
	virtual void UseSkill(void) = 0;
	// Run or walk
	virtual void Patrol(void) = 0;
	virtual void Chase(void) = 0;
	// ジャンプ
	virtual void Jump(void) = 0;
	// ジャンプからの落ちるモーション
	virtual void Fall(void) = 0;
	// 敵の状態を表す--------------------
	// 待機モーション
	virtual void Idle(void);
	// プレイヤーからの攻撃のくらい
	virtual void Hit(void);
	// 死ぬ
	virtual void Death(void);
	// ----------------------------------
private:

protected:

	// 状態の遷移用関数ﾎﾟｲﾝﾀ
	void(Enemy::* stateTransitioner_)(void);
	Player& player_;
	// 自分の攻撃が当たったかのフラグ
	bool hittingToPlayer_;

	// 視界範囲
	float vision_;
	cocos2d::Vec2 oldPos_;
	// 各行動を実施する際の視界範囲
	VisionRange visionRange_;
	// プレイヤーを視認しているかのフラグ
	bool identifiedPlayer_;
	// ビヘイビアツリー
	BehaviorTree* aiTree_;	
	// ビヘイビアデータ
	BehaviorData* behaviorData_;	
	// 実行中ノード
	NodeBase* activeNode_;			

	// 方向変更フラグ
	bool isChangeDir_;

	// アニメーションが変更されたかのフラグ
	bool isChangedAnim_;

	// 移動の種類(patrol or chase)
	MoveType mType_;

	cocos2d::DrawNode* debugCircle_;
	// 一定距離歩いたらtrueになる
	bool isMoveComplete_;
	// 自身の名前
	std::string myName_;
	// 自身の固有ID
	int id_;
};