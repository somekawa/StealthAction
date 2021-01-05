#include "BigCultist.h"
#include "Gravity.h"
#include "_Debug/_DebugConOut.h"
#include "ActionRect.h"
#include "Loader/CollisionLoader.h"
#include "anim/AnimMng.h"
#include "obj/Player.h"

#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"

USING_NS_CC;

BigCultist::BigCultist(cocos2d::Vec2 pos, Player& player, BehaviorTree* aiTree, VisionRange visionRange, int hp, cocos2d::Layer& myLayer) :
	Enemy(pos, player, aiTree, visionRange, hp, myLayer)
{
	this->setScale(3.0f);
	myName_ = "bigCultist";
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::BigCultist;

	// アニメーションの登録
	AnimRegistrator();
	// ActModuleの登録
	//ActModuleRegistrator();

	currentAnimation_ = "bigCultist_idle";
	previousAnimation_ = currentAnimation_;

	//this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_,currentAnimation_)));

	direction_ = Direction::Left;

	//lpCol.Load(collider_, "idle", "assassin");
	// 攻撃矩形のサイズ設定
	attackRect_.size_ = Size(30.0f, 30.0f);

	for (auto anim : lpAnimMng.GetAnimations(type_))
	{
		// colliderBoxのLoad
		lpCol.ReadData(collider_, anim);
		for (auto col : collider_[anim])
		{
			for (int colNum = 0; colNum < col.size(); colNum++)
			{
				// colliderBoxを自身の子にする
				auto draw = col[colNum]->create();
				draw->setContentSize(Size{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y });
				draw->drawRect(Vec2(0, 0), Vec2{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y }, col[colNum]->GetColor());

				draw->setTag(col[colNum]->GetData().frame_);

				this->addChild(draw, col[colNum]->GetData().type_, anim);
			}
		}
	}
}

BigCultist::~BigCultist()
{
}

BigCultist* BigCultist::CreateBigCultist(cocos2d::Vec2 pos, Player& player, BehaviorTree* aiTree, VisionRange visionRange, int hp, cocos2d::Layer& myLayer)
{
	BigCultist* pRet = new(std::nothrow) BigCultist(pos, player, aiTree, visionRange, hp, myLayer);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

void BigCultist::Action(void)
{
}

void BigCultist::update(float delta)
{
	if (getName() == "assassin_death" || getName() == "changeFloor_death")
	{
		// 自分を親であるGameSceneから削除する
		this->removeFromParentAndCleanup(true);
	}
	else
	{
		previousAnimation_ = currentAnimation_;
		if (!isAttacking_)
		{
			// 方向の変更
			ChangeDirection();
		}
		// 現在のフレームを整数値で取得
		animationFrame_int_ = GetAnimationFrameInt() - 1;

		// 0以下になると0にする
		if (animationFrame_int_ < 0)
		{
			animationFrame_int_ = 0;
		}
		// 無条件に通っていたら処理が重くなるので
		// ﾌﾟﾚｲﾔｰが攻撃態勢で自分が攻撃食らっていなかったら(今のところこうしているが、後で自分がhitﾓｰｼｮﾝが終わったらに変更予定)
		if (player_.IsAttacking() && !onDamaged_)
		{
			// ﾌﾟﾚｲﾔｰとの当たり判定をとっている
			CheckHitPLAttack();
		}

		// ﾀﾞﾒｰｼﾞを食らった
		if (onDamaged_ && stateTransitioner_ != &Enemy::Hit)
		{
			// hpを削る
			hp_ -= 10;
			// hpが0になったら
			if (hp_ <= 0)
			{
				if (stateTransitioner_ != &Enemy::Death)
				{
					// 死ぬ状態にする
					ChangeAnimation("death");
					stateTransitioner_ = &Enemy::Death;
				}
			}
			else
			{
				// 0ではなかったらhit状態にする
				ChangeAnimation("hit");
				stateTransitioner_ = &Enemy::Hit;
			}
		}

		//if (currentAnimation_ == "attack")
		{
		}

		//previousAnimation_ = currentAnimation_;

		TRACE("pos:(%f,%f)", this->getPosition().x, this->getPosition().y);

		TRACE("attackFlag:%d\n", isAttacking_);
		if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
		{
			animationFrame_ = 0.0f;
			return;
		}


		for (auto animationCol = this->getChildren().rbegin();
			animationCol != this->getChildren().rend(); animationCol++)
		{
			if (currentAnimation_ == (*animationCol)->getName() &&
				animationFrame_int_ == (*animationCol)->getTag())
			{
				(*animationCol)->setVisible(true);
			}
			else
			{
				(*animationCol)->setVisible(false);
			}
		}
		// 状態の遷移
		(this->*stateTransitioner_)();
		// Mapオブジェクトに当たっているかの確認
		//CheckMapObjHit(delta);
		// 重力をかける
		gravity_->ApplyGravityToTarget(delta);
		// アニメーションの更新
		UpdateAnimation(delta);
		// アニメーション終了時に攻撃フラグをfalse
		if (isAnimEnd_)
		{
			isAttacking_ = false;
			hittingToPlayer_ = false;
		}

	}

}

void BigCultist::AnimRegistrator(void)
{
}

void BigCultist::AddAttackObj(const float& angle)
{
}

void BigCultist::NormalAttack(void)
{
	isAttacking_ = true;
	if (animationFrame_int_ < 21)
	{
		currentCol_ = collider_[currentAnimation_][animationFrame_int_];
	}
	if (OnAttacked())
	{
		player_.OnDamaged();
	}
	TRACE("direction:%d", direction_);
}

void BigCultist::UseSkill(void)
{
}

void BigCultist::Patrol(void)
{
	if (isMoveComplete_)
	{
		// 一定距離歩いていたら
		oldPos_ = getPosition();
		isMoveComplete_ = false;
	}
	else
	{
		// 歩いた距離がある一定距離になるまで
		if (getPosition().x >= oldPos_.x + 30.0f)
		{
			isMoveComplete_ = true;
		}
	}
	auto previousPos = getPosition().x;
	// 巡回処理
	switch (direction_)
	{
	case Direction::Right:
		speed_.x = 1;
		flipFlag_ = FlipX::create(true);
		break;
	case Direction::Left:
		speed_.x = -1;
		flipFlag_ = FlipX::create(false);
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
	currentAnimation_ = "bigCultist_run";
	auto move = MoveTo::create(0.0f, Vec2(previousPos + speed_.x, getPosition().y));
	//pos_.x += speed_.x;
	//this->setPosition(Vec2(pos_.x, pos_.y));
	this->runAction(move);
	this->runAction(flipFlag_);
	setPosition(Vec2(previousPos + speed_.x, getPosition().y));
}

void BigCultist::Chase(void)
{
	auto previousPos = getPosition().x;

	// プレイヤーを追いかける処理
	switch (direction_)
	{
	case Direction::Right:
		speed_.x = 2;
		flipFlag_ = FlipX::create(true);
		break;
	case Direction::Left:
		speed_.x = -2;
		flipFlag_ = FlipX::create(false);
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
	currentAnimation_ = "bigCultist_run";
	auto move = MoveTo::create(0.0f, Vec2(previousPos + speed_.x, getPosition().y));
	//pos_.x += speed_.x;
	//this->setPosition(Vec2(pos_.x, pos_.y));
	this->runAction(move);
	this->runAction(flipFlag_);
	setPosition(Vec2(previousPos + speed_.x, getPosition().y));
}

void BigCultist::Run(void)
{
}

void BigCultist::Jump(void)
{
}

void BigCultist::Fall(void)
{
}
