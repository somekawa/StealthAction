#include "obj/Enemy/Enemy.h"
#include "Scene/GameScene.h"
#include "obj/Player.h"
#include "Gravity.h"
#include "ActionRect.h"
#include "obj/Player.h"
#include "EnemyHPGauge.h"
#include "BehaviorBaseAI/BehaviorTree.h"
#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"
#include "_Debug/_DebugConOut.h"

USING_NS_CC;

Enemy::Enemy(Vec2 pos,Player& player, BehaviorTree* aiTree,VisionRange visionRange,int hp,Layer& myLayer):
	Actor(hp,myLayer),player_(player),aiTree_(aiTree),activeNode_(NULL),behaviorData_(NULL),visionRange_(visionRange)
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
	//onExit();
}

const float& Enemy::GetVision(void)
{
	// ﾌﾟﾚｲﾔｰのﾎﾟｼﾞｼｮﾝ
	auto plPos = player_.getPosition();
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
	auto flip = false;
	auto flipBit = 0;
	if (!isMove_[static_cast<int>(direction_)])
	{
		if (direction_ == Direction::Right)
		{
			flipBit = 1;
		}
		else
		{
			flipBit = -1;
		}
		direction_ = (direction_ + flipBit);
		isMove_[static_cast<int>(direction_)] = true;
	}
	else
	{
		if (mType_ == MoveType::Chase)
		{
			// ﾌﾟﾚｲﾔｰのﾎﾟｼﾞｼｮﾝ
			auto playerPos = player_.getPosition();
			// 1ﾌﾚｰﾑ前の自身の向いている方向の格納
			oldDirection_ = direction_;
			// ﾌﾟﾚｲﾔｰが自身よりも左にいる場合
			if (getPosition().x > playerPos.x)
			{
				direction_ = Direction::Left;
			}
			// ﾌﾟﾚｲﾔｰが自身よりも右にいる場合
			else
			{
				direction_ = Direction::Right;
			}
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
				direction_ = (Direction)RandomHelper::random_int((int)Direction::Right, (int)Direction::Left);
			}
		}
	}

	if (direction_ == Direction::Left)
	{
		flip = false;
	}
	else if (direction_ == Direction::Right)
	{
		flip = true;
	}
	// patrol(巡回)行動でない場合はpatrolFrameをｾﾞﾛｸﾘ
	//if (mType_ != MoveType::Patrol)
	//{
	//	patrolFrame_ = 0.0f;
	//}
	runAction(FlipX::create(flip));
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
						// 方向によって矩形ﾎﾟｼﾞｼｮﾝが変更するので、変更したﾎﾟｼﾞｼｮﾝに応じてbeginを変更
						attackColData.begin_ = Vector2I(myCol->getPosition().x,myCol->getPosition().y);

						auto attackColPos = Vec2{ getPosition().x + (attackColData.begin_.x + (attackColData.size_.x/2)),
												  getPosition().y + (attackColData.begin_.y + (attackColData.size_.y/2)) };
						auto damageColPos = Vec2{ player_.getPosition().x + (plDamageCol.begin_.x + (plDamageCol.size_.x/2)) ,
												  player_.getPosition().y + (plDamageCol.begin_.y + (plDamageCol.size_.y/2)) };
						// 当たり判定実施
						if (abs(attackColPos.x - damageColPos.x) <= visionRange_.attack_ + 10.0f &&
							abs(attackColPos.y - damageColPos.y) <= 80.0f)
						{
							auto t = type_;
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

void Enemy::CheckHitPLAttack(void)
{
	if (!onDamaged_)
	{
		// ﾌﾟﾚｲﾔｰの現在のｺﾘｼﾞｮﾝﾃﾞｰﾀ
		auto plColData = player_.GetCurrentCol();
		// ﾌﾟﾚｲﾔｰの現在のｺﾘｼﾞｮﾝﾃﾞｰﾀで回す
		for (auto attackCol : plColData)
		{
			// 現在のﾌﾟﾚｲﾔｰのｺﾘｼﾞｮﾝﾃﾞｰﾀの中に攻撃矩形が有れば
			if (attackCol->GetData().type_ == 0)
			{
				// 現在の自分のｺﾘｼﾞｮﾝﾃﾞｰﾀで回す
				for (auto myCol : currentCol_)
				{
					// 現在の自分のｺﾘｼﾞｮﾝﾃﾞｰﾀの中にﾀﾞﾒｰｼﾞ矩形が有れば
					if (myCol->GetData().type_ == 1)
					{
						// ﾌﾟﾚｲﾔｰのﾎﾟｼﾞｼｮﾝが現在どこか不明
						auto plPos = player_.getPosition();
						// 攻撃矩形のﾎﾟｼﾞｼｮﾝ(真ん中に設定)
						auto attackColPos = Vec2{ player_.getPosition().x + attackCol->GetData().begin_.x + (attackCol->GetData().size_.x / 2),
												  player_.getPosition().y + attackCol->GetData().begin_.y + (attackCol->GetData().size_.y / 2) };
						// ﾀﾞﾒｰｼﾞ矩形のﾎﾟｼﾞｼｮﾝ(真ん中に設定)
						auto damageColPos = Vec2{ getPosition().x + myCol->GetData().begin_.x + (myCol->GetData().size_.x / 2),
												  getPosition().y + myCol->GetData().begin_.y + (myCol->GetData().size_.y / 2) };
						// 攻撃矩形とﾀﾞﾒｰｼﾞ矩形の距離が50以下だと当たっている判定に
						if (abs(attackColPos.x - damageColPos.x) <= 60.0f &&
							abs(attackColPos.y - damageColPos.y) <= 80.0f)
						{
							hp_ -= 10;
							// HP減少のテストコード
							auto a = ((Game*)Director::getInstance()->getRunningScene());
							auto b = (EnemyHPGauge*)a->getChildByTag((int)zOlder::FRONT)->getChildByName(myName_ + "_" + std::to_string(id_) + "_HP");
							b->SetHP(hp_);	// -10などのダメージ量は敵の攻撃力に変えればいい
							// onDamaged_をtrueに
							OnDamaged();
						}
					}
				}
			}
		}
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
	animationFrame_ += delta;
	//TRACE("%s", currentAnimation_);
	//TRACE("currentAnimation:%s,animFrame:%f\n", currentAnimation_.c_str(), animationFrame_);
	// あるアニメーションの終了までの継続時間の格納
	auto duration = lpAnimMng.GetAnimationCache(type_, currentAnimation_)->getDuration();
	// アニメーションカウントが継続時間を超えていれば
	if (animationFrame_ >= duration)
	{
		// ループフラグがtrueの場合はループ再生
		if (lpAnimMng.GetIsLoop(type_, currentAnimation_))
		{
			//auto action = RepeatForever::create(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
			//this->runAction(action);
			animationFrame_ = 0.0f;
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
	//if (currentAnimation_ == animName)
	//{
	//	return;
	//}
	// 現在のアニメーションを変更先のアニメーション名に変更
	currentAnimation_ = animName;
	animationFrame_ = 0.0f;
	animationFrame_int_ = 0;
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

const float& Enemy::GetDistance(void)
{
	// ﾌﾟﾚｲﾔｰのﾎﾟｼﾞｼｮﾝ
	auto playerPos = player_.getPosition();
	// ﾌﾟﾚｲﾔｰのﾎﾟｼﾞｼｮﾝと自身のﾎﾟｼﾞｼｮﾝを引いた数をabsした数字を返す
	return abs(playerPos.x - getPosition().x);
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

void Enemy::Hit(void)
{
	TRACE("Hit!\n");



	if (isAnimEnd_)
	{
		onDamaged_ = false;
	}
}

void Enemy::Death(void)
{
	TRACE("death");
	// deatthﾓｰｼｮﾝが終了していたら
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
	//AIRun();
	if (isAnimEnd_)
	{
		// またﾀﾞﾒｰｼﾞを与えられるようにするためにfalseにしておく
		onDamaged_ = false;
		currentAnimation_ = myName_ + "_idle";
		//stateTransitioner_ = &Enemy::Idle;
	}
	//currentAnimation_ = myName_ + "_idle";
}
