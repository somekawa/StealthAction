#include "obj/Enemy/Enemy.h"
#include "Scene/GameScene.h"
#include "obj/Player.h"
#include "BehaviorBaseAI/BehaviorTree.h"
#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"
#include "_Debug/_DebugConOut.h"

USING_NS_CC;

Enemy::Enemy(Vector<Node*>& player, BehaviorTree* aiTree):
	player_(player),aiTree_(aiTree),activeNode_(NULL),behaviorData_(NULL)
{
	// ビヘイビアデータの生成
	behaviorData_ = new BehaviorData();
	// 攻撃しているかのフラグの初期化
	isAttacking_ = false;
	// アニメーションが変更されたかのフラグの初期化
	isChangedAnim_ = false;

	isAnimEnd_ = false;
}

Enemy::~Enemy()
{
	//onExit();
}

void Enemy::ChangeDirection(void)
{
	auto playerPos = player_.front()->getPosition();

	if (pos_.x > playerPos.x)
	{
		direction_ = Direction::Left;
	}
	else
	{
		direction_ = Direction::Right;
	}
}

// アニメーションの更新関数(もうちょっと改良する)
void Enemy::UpdateAnimation(float delta)
{
	/*if (currentAnimation_ != previousAnimation_)
	{
		animationFrame_ = lpAnimMng.GetFrameNum(type_,currentAnimation_);
	}*/
	// アニメーションカウントを毎フレームdelta値を加算
	animationFrame_ -= delta * 10;
	TRACE("%s", currentAnimation_);
	TRACE("currentAnimation:%s,animFrame:%f\n", currentAnimation_.c_str(), animationFrame_);
	// あるアニメーションの終了までの継続時間の格納
	auto duration = lpAnimMng.GetAnimationCache(type_, currentAnimation_)->getDuration();
	// アニメーションカウントが継続時間を超えていれば
	if (animationFrame_ <= 0.0f)
	{
		// ループフラグがtrueの場合はループ再生
		if (lpAnimMng.GetIsLoop(type_, currentAnimation_))
		{
			//auto action = RepeatForever::create(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
			//this->runAction(action);
			animationFrame_ = lpAnimMng.GetFrameNum(type_, currentAnimation_);
			//animationFrame_ = 0.0f;
		}
		else
		{
			// falseの場合は1回の再生
			// 1アニメーションが終了したフラグを立てる
			isAnimEnd_ = true;
			//animationFrame_ = 0.0f;
		}
	}
	TRACE("animEndFlag:%d\n", isAnimEnd_);

}

void Enemy::ChangeAnimation(std::string animName)
{
	// 今の動きを止める
	this->stopAllActions();

	// 変更したいアニメーションが現在のアニメーションと同じならば何もしない。
	/*if (currentAnimation_ == animName)
	{
		return;
	}*/
	// 現在のアニメーションを変更先のアニメーション名に変更
	currentAnimation_ = animName;
	animationFrame_ = lpAnimMng.GetFrameNum(type_, currentAnimation_);

	// アニメーション終了フラグをfalseに
	isAnimEnd_ = false;
	// アニメーションのフレーム数を初期値に戻す
	//animationFrame_ = 0.0f;
	if (lpAnimMng.GetIsLoop(type_, currentAnimation_))
	{
		auto action = RepeatForever::create(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
		this->runAction(action);
	}
	else
	{
		this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
	}
}

const float& Enemy::DistanceCalcurator(void)
{
	auto playerPos = player_.front()->getPosition();

	return abs(playerPos.x - pos_.x);
}

void Enemy::SetIsAttacking(bool flg)
{
	isAttacking_ = flg;
}

void Enemy::AIRun(void)
{
	if (activeNode_ == NULL)
	{
		activeNode_ = aiTree_->Inference(this, behaviorData_);
	}

	if (activeNode_ != NULL)
	{
		activeNode_ = aiTree_->Run(this, activeNode_, behaviorData_);
	}
}

void Enemy::SetMoveType(MoveType type)
{
	mType_ = type;
}
