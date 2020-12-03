#pragma once
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

class Enemy :
	public Actor
{
public:
	Enemy(cocos2d::Vector<cocos2d::Node*>& player_, BehaviorTree* aiTree);
	~Enemy();

	virtual void Action(void) = 0;
	virtual void ChangeDirection(void);
	virtual const float& DistanceCalcurator(void);

	void SetIsAttacking(bool flg);

	// 攻撃しているかのフラグ取得関数
	virtual const bool& IsAttacking(void)
	{
		return isAttacking_;
	}
	// 自分の行動タイプの取得
	virtual const MoveType& GetMoveType(void)
	{
		return mType_;
	}

	// アニメーションの更新
	virtual void UpdateAnimation(float delta);
	// アニメーションの変更
	// param@ animName: 変更先アニメーション名
	virtual void ChangeAnimation(std::string animName);

	virtual void AnimRegistrator(void) = 0;
	// AI行動のNodeを探索して実行
	virtual void AIRun(void);
	// 行動タイプのセット
	virtual void SetMoveType(MoveType type);
	
	// 物理攻撃
	virtual void NormalAttack(void) = 0;
	// スキル使用
	virtual void UseSkill(void) = 0;
	// 待機モーション
	virtual void Idle(void) = 0;
	// Run or walk
	virtual void Patrol(void) = 0;
	virtual void Chase(void) = 0;
	// ジャンプ
	virtual void Jump(void) = 0;
	// ジャンプからの落ちるモーション
	virtual void Fall(void) = 0;
	// プレイヤーからの攻撃のくらい
	virtual void Hit(void) = 0;
	// 死ぬ
	virtual void Death(void) = 0;
private:

protected:
	cocos2d::Vector<cocos2d::Node*>& player_;

	BehaviorTree* aiTree_;			// ビヘイビアツリー
	BehaviorData* behaviorData_;	// ビヘイビアデータ
	NodeBase* activeNode_;			// 実行中ノード

	// 攻撃しているかのフラグ
	bool isAttacking_;
	// アニメーションが変更されたかのフラグ
	bool isChangedAnim_;

	MoveType mType_;
};