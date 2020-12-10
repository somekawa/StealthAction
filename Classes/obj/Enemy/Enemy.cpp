#include "obj/Enemy/Enemy.h"
#include "Scene/GameScene.h"
#include "obj/Player.h"
#include "Gravity.h"
#include "ActionRect.h"
#include "obj/Player.h"
#include "BehaviorBaseAI/BehaviorTree.h"
#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"
#include "_Debug/_DebugConOut.h"

USING_NS_CC;

Enemy::Enemy(Player& player, BehaviorTree* aiTree,VisionRange visionRange,int hp):
	Actor(hp),player_(player),aiTree_(aiTree),activeNode_(NULL),behaviorData_(NULL),visionRange_(visionRange)
{
	// ビヘイビアデータの生成
	behaviorData_ = new BehaviorData();
	// 攻撃しているかのフラグの初期化
	isAttacking_ = false;
	// アニメーションが変更されたかのフラグの初期化
	isChangedAnim_ = false;
	// プレイヤーを視認したかのフラグの初期化
	identifiedPlayer_ = false;
	// 方向変更フラグの初期化
	isChangeDir_ = false;
	isAnimEnd_ = false;
	isMoveComplete_ = true;
	// 自分の攻撃が当たったフラグの初期化
	hittingToPlayer_ = false;

	oldPos_ = getPosition();
	// プレイヤーと自分の距離を測ってインスタンス
	vision_ = abs(player_.getPosition().x - getPosition().x);
}

Enemy::~Enemy()
{
	//onExit();
}

const float& Enemy::GetVision(void)
{
	auto plPos = player_.getPosition();

	vision_ = abs(plPos.x - getPosition().x);
	return vision_;
}

void Enemy::ChangeDirection(void)
{
	auto playerPos = player_.getPosition();

	if (getPosition().x > playerPos.x)
	{
		direction_ = Direction::Left;
	}
	else
	{
		direction_ = Direction::Right;
	}
}

bool Enemy::OnAttacked(void)
{
	// ﾌﾟﾚｲﾔｰに攻撃を1回のみ当てたいので
	// 攻撃が1回当たれば当たり判定は切る
	if (!hittingToPlayer_)
	{
		// ﾌﾟﾚｲﾔｰのｺﾗｲﾀﾞｰﾎﾞｯｸｽの走査
		for (auto plCol : player_.GetCurrectCol())
		{
			// ﾀﾞﾒｰｼﾞ判定ｺﾗｲﾀﾞｰﾎﾞｯｸｽだったら
			if (plCol->GetData().type_ == 1)
			{
				// 自分のｺﾗｲﾀﾞｰﾎﾞｯｸｽの走査
				for (auto myCol : currentCol_)
				{
					// 攻撃判定ｺﾗｲﾀﾞｰﾎﾞｯｸｽだったら
					if (myCol->GetData().type_ == 0)
					{
						// ﾌﾟﾚｲﾔｰの当たり判定対象のﾎﾞｯｸｽをﾀﾞﾒｰｼﾞｺﾗｲﾀﾞｰﾎﾞｯｸｽﾃﾞｰﾀを格納
						auto plDamageCol = plCol->GetData();
						// 自分の当たり判定のｺﾗｲﾀﾞｰﾎﾞｯｸｽﾃﾞｰﾀにattackｺﾗｲﾀﾞｰﾎﾞｯｸｽﾃﾞｰﾀを格納
						auto attackColData = myCol->GetData();
						// 当たり判定実施
						if ((attackColData.begin_.x + (attackColData.size_.x / 2) -
							(player_.getPosition().x + (plDamageCol.begin_.x + (plDamageCol.size_.x / 2)) <= 0) &&
							(attackColData.begin_.y + (attackColData.size_.y / 2) -
								(player_.getPosition().y + (plDamageCol.begin_.y + (plDamageCol.size_.y / 2)) <= 0))))
						{
							// 攻撃を当てたtriggerをtrueにする
							hittingToPlayer_ = true;
							return true;
						}
					}
				}
			}
		}
	}
	// 攻撃が当たっていない場合何もしない
	return false;
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
	auto playerPos = player_.getPosition();

	return abs(playerPos.x - getPosition().x);
}

void Enemy::SetIsAttacking(bool flg)
{
	isAttacking_ = flg;
}

void Enemy::AIRun(void)
{
	// ダメージくらい
	// ダメージが当たればtrueになる
	//onDamaged_ = CheckHitAttack(player_.GetAttackRect());
	//// ダメージテスト
	//if (onDamaged_)
	//{
	//	int i = 0;
	//}

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

bool Enemy::CheckObjHit(void)
{
	auto director = Director::getInstance();
	auto a = director->getRunningScene()->getChildByTag((int)zOlder::BG);
	if ((TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData") == nullptr)
	{
		return false;
	}
	const int chipSize = 48;
	auto CollisionData = (TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData")->getChildByName("col");
	auto ColSize = CollisionData->getLayerSize();

	auto position = this->getPosition();

	auto nextPos = [&]() {
		Vec2 next;
		switch (direction_)
		{
		case Direction::Right:
			next = { position.x + (this->getContentSize().width / 2) + speed_.x,position.y };
			break;
		case Direction::Left:
			next = { position.x - (this->getContentSize().width / 2) + speed_.x,position.y };

			break;
		case Direction::Up:
			break;
		case Direction::Down:
			break;
		case Direction::Max:
			break;
		default:
			break;
		}
		return next;
	};

	auto grid = Vec2{ nextPos().x,nextPos().y } / chipSize;

	auto gridPos = Vec2(grid.x, ColSize.height - grid.y);

	// 範囲外check
	if (gridPos.x > ColSize.width || gridPos.x < 0 ||
		gridPos.y > ColSize.height || gridPos.y < 0)
	{
		return true;
	}
	auto gridGid = CollisionData->getTileGIDAt(gridPos);
	if (gridGid != 0)
	{
		//sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - (module.flame));
		return true;
	}

	return false;
}
