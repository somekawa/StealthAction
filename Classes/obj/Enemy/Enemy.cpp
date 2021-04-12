#include "_Debug/_DebugConOut.h"
#include "Scene/GameScene.h"
#include "obj/Player.h"
#include "Gravity.h"
#include "ActionRect.h"
#include "../../HPGauge.h"
#include "BehaviorBaseAI/BehaviorTree.h"
#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"
#include "Enemy.h"

USING_NS_CC;

Enemy::Enemy(Vec2 pos,Player& player, BehaviorTree* aiTree,VisionRange visionRange,int hp,Layer& myLayer):
	Actor(hp,myLayer),player_(player),aiTree_(aiTree),activeNode_(nullptr),behaviorData_(nullptr),visionRange_(visionRange)
{
	retain();
	// ビヘイビアデータの生成
	behaviorData_ = new BehaviorData();
	setPosition(pos);
	// アニメーションが変更されたかのフラグの初期化
	isChangedAnim_ = false;
	isAttacking_ = false;
	// プレイヤーを視認したかのフラグの初期化
	identifiedPlayer_ = false;
	// 方向変更フラグの初期化
	isChangeDir_ = false;
	isAnimEnd_ = false;
	isMoveComplete_ = true;
	// 自分の攻撃が当たったフラグの初期化
	hittingToPlayer_ = false;
	// 動けるかのﾌﾗｸﾞの初期化
	for (auto& flg : isMove_)
	{
		flg = true;
	}
	// 状態遷移の関数ﾎﾟｲﾝﾀの初期化
	stateTransitioner_ = &Enemy::Idle;
	// moveTypeの初期化
	mType_ = MoveType::Non;
	oldPos_ = getPosition();
	// patrol(巡回行動)をしているﾌﾚｰﾑ数の初期化
	patrolFrame_ = 0.0f;
	// プレイヤーと自分の距離を測ってインスタンス
	vision_ = abs(player_.getPosition().x - getPosition().x);
}

Enemy::~Enemy()
{
}

const float& Enemy::GetVision(void)
{
	// ﾌﾟﾚｲﾔｰのﾎﾟｼﾞｼｮﾝ
	const Vec2 plPos = player_.getPosition();
	// 現在の敵の視覚情報(ﾌﾟﾚｲﾔｰが自分に対してどのくらいの所にいるか)
	vision_ = abs(plPos.x - getPosition().x);
	return vision_;
}

void Enemy::DeleteSelfOnFloor(void)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		removeFromParentAndCleanup(true);
		return;
	}
}

void Enemy::ChangeDirection(float delta)
{
	int flipBit = 0;
	if (!isMove_[static_cast<int>(direction_)])
	{
		direction_ == Direction::Right ? flipBit = 1 : flipBit = -1;
		direction_ = (direction_ + flipBit);
		isMove_[static_cast<int>(direction_)] = true;
	}
	else
	{
		if (mType_ == MoveType::Chase)
		{
			// ﾌﾟﾚｲﾔｰのﾎﾟｼﾞｼｮﾝ
			const Vec2 playerPos = player_.getPosition();
			// 1ﾌﾚｰﾑ前の自身の向いている方向の格納
			oldDirection_ = direction_;
			// ﾌﾟﾚｲﾔｰが自身よりもどちらにいるかで方向を決める
			direction_ = (getPosition().x > playerPos.x ? Direction::Left : Direction::Right);
		}
		else if (mType_ == MoveType::Patrol)
		{
			// 1ﾌﾚｰﾑ前の自身の向いている方向の格納
			oldDirection_ = direction_;
			// patrolをしている間のｶｳﾝﾄを加算
			patrolFrame_ += delta;
			// patrol(巡回)行動をしているのが一定間隔以上だったら
			if (patrolFrame_ >= DefPatrolFrame)
			{
				patrolFrame_ = 0.0f;
				// 右か左の方向をﾗﾝﾀﾞﾑで決定する
				// cocos2d::RandomHelper::random_int(int min,int max);
				// min(最小)～max(最大)の間でﾗﾝﾀﾞﾑな数値を返す
				direction_ = (Direction)RandomHelper::random_int(static_cast<int>(Direction::Right), static_cast<int>(Direction::Left));
			}
		}
		else
		{
			// 何も処理を行わない
		}
	}

	runAction(FlipX::create(direction_ == Direction::Left ? false : true));
}

bool Enemy::OnAttacked(void)
{
	// ﾌﾟﾚｲﾔｰに攻撃を1回のみ当てたいので
	// 攻撃が1回当たれば当たり判定は切る
	if (!hittingToPlayer_)
	{
		// ﾌﾟﾚｲﾔｰのｺﾗｲﾀﾞｰﾎﾞｯｸｽの走査
		for (auto plCol : player_.GetCurrentCol())
		{
			if (plCol->GetData().type_ != 1)
			{
				// ﾀﾞﾒｰｼﾞ判定ｺﾗｲﾀﾞｰﾎﾞｯｸｽ以外なら何もしない
				continue;
			}
			// 自分のｺﾗｲﾀﾞｰﾎﾞｯｸｽの走査
			for (auto myCol : currentCol_)
			{
				if (myCol->GetData().type_ != 0)
				{
					// 攻撃判定ｺﾗｲﾀﾞｰﾎﾞｯｸｽ以外なら何もしない
					continue;
				}
				// ﾌﾟﾚｲﾔｰの当たり判定対象のﾎﾞｯｸｽをﾀﾞﾒｰｼﾞｺﾗｲﾀﾞｰﾎﾞｯｸｽﾃﾞｰﾀを格納
				const auto plDamageCol = plCol->GetData();
				// 自分の当たり判定のｺﾗｲﾀﾞｰﾎﾞｯｸｽﾃﾞｰﾀにattackｺﾗｲﾀﾞｰﾎﾞｯｸｽﾃﾞｰﾀを格納
				auto attackColData = myCol->GetData();
				// 方向によって矩形ﾎﾟｼﾞｼｮﾝが変更するので、変更したﾎﾟｼﾞｼｮﾝに応じてbeginを変更
				attackColData.begin_ = Vector2I(static_cast<int>(myCol->getPosition().x),static_cast<int>(myCol->getPosition().y));

				const Vec2 attackColPos{ getPosition().x + (attackColData.begin_.x + (attackColData.size_.x/2.0f)),
										 getPosition().y + (attackColData.begin_.y + (attackColData.size_.y/2.0f)) };
				const Vec2 damageColPos{ player_.getPosition().x + (plDamageCol.begin_.x + (plDamageCol.size_.x/2.0f)) ,
										 player_.getPosition().y + (plDamageCol.begin_.y + (plDamageCol.size_.y/2.0f)) };
				// 当たり判定実施
				if (abs(attackColPos.x - damageColPos.x) <= visionRange_.attack_ + 10.0f &&
					abs(attackColPos.y - damageColPos.y) <= 80.0f)
				{
					// 攻撃を当てたtriggerをtrueにする
					hittingToPlayer_ = true;
					return true;
				}
			}
		}
	}
	// 攻撃が当たっていない場合何もしない
	return false;
}

const float Enemy::GetDistance(void)
{
	// ﾌﾟﾚｲﾔｰのﾎﾟｼﾞｼｮﾝと自身のﾎﾟｼﾞｼｮﾝを引いた数をabsした数字を返す
	return abs(player_.getPosition().x - getPosition().x);
}

void Enemy::SetAlive(bool flg)
{
	isAlive_ = flg;
}

void Enemy::SetID(int id)
{
	id_ = id;
}

void Enemy::AIRun(void)
{
	// activeStateがNULLの場合はbehaviorTreeから推論し、aiTreeに行動を格納
	if (activeNode_ == NULL)
	{
		activeNode_ = aiTree_->Inference(this, behaviorData_);
	}
	// activeNodeがNULLでない場合は敵の行動として追加したノード
	// から条件に合致する行動を実行
	if (activeNode_ != NULL)
	{
		activeNode_ = aiTree_->Run(this, activeNode_, behaviorData_);
	}
}

void Enemy::SetMove(const Direction& dir,bool flg)
{
	isMove_[static_cast<int>(dir)] = flg;
}

void Enemy::SetMoveType(MoveType type)
{
	mType_ = type;
}

void Enemy::Hit(void)
{
	TRACE("Hit!\n");
	const auto director = Director::getInstance();
	const auto scene = director->getRunningScene();
	const auto hpGauge = (HPGauge*)scene->getChildByTag(static_cast<int>(zOrder::FRONT))->getChildByName(getName() + "_HPgauge" + "_" + std::to_string(id_));

	if (currentAnimation_ == getName() + "_hit")
	{
		if (previousAnimation_ == currentAnimation_)
		{
			return;
		}
	}

	if (isHitAttack_)
	{
		const auto player = scene->getChildByTag(static_cast<int>(zOrder::CHAR_PL))->getChildByName("player1");
		hp_ -= static_cast<Player*>(player)->GetGiveDamageNum();
		hpGauge->SetHP(static_cast<float>(hp_));
		isHitAttack_ = false;
	}
}

void Enemy::Death(void)
{
	TRACE("death");
	// deathﾓｰｼｮﾝが終了していたら
	if (isAnimEnd_)
	{
		// 自身を消去するために自分の名前をdeathにする(仮でこうしている)
		setName(myName_ + "_death");
		// 生きているﾌﾗｸﾞをfalseに(死んだ事にする)
		isAlive_ = false;
	}
}

void Enemy::Idle(void)
{
	if (isHitAttack_ && (previousAnimation_ != currentAnimation_))
	{
		stateTransitioner_ = &Enemy::Hit;
	}
	if (isAnimEnd_)
	{
		// またﾀﾞﾒｰｼﾞを与えられるようにするためにfalseにしておく
		isHitAttack_ = false;
		currentAnimation_ = myName_ + "_idle";
	}
}