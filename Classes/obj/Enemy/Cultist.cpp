#include "_Debug/_DebugConOut.h"
#include "Gravity.h"
#include "ActionRect.h"
#include "Loader/CollisionLoader.h"
#include "AnimationEnum/AnimationType.h"
#include "anim/AnimMng.h"
#include "SoundMng.h"
#include "obj/Player.h"
#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"
#include "obj/Objects/Fireball.h"
#include "Cultist.h"

USING_NS_CC;

Cultist::Cultist(Vec2 pos, Player& player, BehaviorTree* aiTree, VisionRange visionRange, int hp,Layer& myLayer):
    Enemy(pos,player, aiTree, visionRange, hp,myLayer)
{
	attackCnt_ = 0;
	myName_ = "cultist";
	flag_   = false;
	fireBallCT_ = 0;

	lpSoundMng.AddSound("burst01", "BGM/s-burst01.mp3",SoundType::SE);

	this->setScale(2.0f);
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::Cultist;

	// アニメーションの登録
	ActModuleRegistration();

	currentAnimation_ = "cultist_idle";
	previousAnimation_ = currentAnimation_;

	AnimRegistrator();
	this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));

	direction_ = Direction::Left;

	// 攻撃矩形のサイズ設定
	attackRect_.size_ = Size(30.0f, 30.0f);

	for (auto anim : lpAnimMng.GetAnimations(type_))
	{
		// colliderBoxのLoad
		lpCol.ReadData(collider_, anim);
	}

	// 初期アニメーションのセット
	lpAnimMng.InitAnimation(*this, ActorType::Cultist, "cultist_idle");
}

Cultist::~Cultist()
{
}

Cultist* Cultist::CreateCultist(Vec2 pos, Player& player, BehaviorTree* aiTree, VisionRange visionRange, int hp,Layer& myLayer)
{
	Cultist* pRet = new(std::nothrow) Cultist(pos,player, aiTree, visionRange, hp,myLayer);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

void Cultist::update(float delta)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		animationFrame_ = 0.0f;
		return;
	}
	// 死んだ判定
	if (getName() == "cultist_death" || getName() == "changeFloor_death")
	{
		attackLayer_->removeFromParentAndCleanup(true);
		// 自分を親であるGameSceneから削除する
		this->removeFromParentAndCleanup(true);
	}
	else
	{
		setAnchorPoint(Vec2(0.5f, 0.0f));
		if (!isAttacking_)
		{
			// 方向の変更
			ChangeDirection(delta);
		}
		// 現在のフレームを整数値で取得
		animationFrame_int_ = GetAnimationFrameInt() - 1;

		// 0以下になると0にする
		if (animationFrame_int_ < 0)
		{
			animationFrame_int_ = 0;
		}
		// ﾀﾞﾒｰｼﾞをくらっていない時と死ぬﾓｰｼｮﾝでない場合
		if (!isHitAttack_ && stateTransitioner_ != &Enemy::Death)
		{
			// actCtlのｱｯﾌﾟﾃﾞｰﾄを回す
			actCtl_.Update(type_, *this);
		}


		if (stateTransitioner_ != &Enemy::Death)
		{
			// ﾀﾞﾒｰｼﾞを食らった
			if (isHitAttack_)
			{
				if (stateTransitioner_ != &Enemy::Hit)
				{
					ChangeAnimation("cultist_hit");
					stateTransitioner_ = &Enemy::Hit;
				}
			}
		}
		// hpが0になったら
		if (hp_ <= 0)
		{
			if (stateTransitioner_ != &Enemy::Death)
			{
				ChangeAnimation("cultist_death");
				stateTransitioner_ = &Enemy::Death;
			}
		}

		// アニメーションの更新
		UpdateAnimation(delta);
		// 状態の遷移
		(this->*stateTransitioner_)();

		// アニメーション終了時に攻撃フラグをfalse
		if (isAnimEnd_)
		{
			isHitAttack_ = false;
			isAttacking_ = false;
			hittingToPlayer_ = false;
			stateTransitioner_ = &Enemy::Idle;
			currentAnimation_  = "cultist_idle";
		}

		if (currentAnimation_ != previousAnimation_)
		{
			ChangeAnimation(currentAnimation_);
		}
		previousAnimation_ = currentAnimation_;
		// 各矩形情報のｾｯﾄ
		SetCollider();
	}
}

bool Cultist::AddAttackObj(const float& angle)
{
	// 連続で攻撃するのを防ぐ
	if (flag_)
	{
		// 1度攻撃した後に再度攻撃するまでのクールタイムを設定する
		if (fireBallCT_ > 0)
		{
			fireBallCT_--;
		}
		else
		{
			fireBallCT_ = 0;
			flag_ = false;
		}
		currentAnimation_ = "cultist_idle";
		isAttacking_ = false;
		return false;
	}

	lpSoundMng.PlayBySoundName("burst01");
	flag_ = true;
	fireBallCT_ = 120;

	// 攻撃していたらfireballを飛ばす
	auto fireball = Fireball::CreateFireball({ getPosition().x,getPosition().y + 30.0f },direction_,angle,player_);
	fireball->runAction(FlipX::create((direction_ == Direction::Left ? false : true)));
	// 攻撃ﾚｲﾔｰにfireballをぶら下げる
	attackLayer_->addChild(fireball,0);
	attackCnt_++;
	isAttacking_ = true;
	fireball->scheduleUpdate();
	return true;
}

const float Cultist::GetPLAngle(void)
{
	const auto plPos = player_.getPosition();
	const auto angle = atan2(getPosition().y - plPos.y,getPosition().x - plPos.x);
	return angle;
}

void Cultist::ActModuleRegistration(void)
{
	Actor::XmlActDataRead("cultistModule", nullptr);
}

void Cultist::NormalAttack(void)
{
	if (animationFrame_int_ < 11)
	{
		currentCol_ = collider_[currentAnimation_][animationFrame_int_];
	}

	if (animationFrame_ >= 0.01f)
	{
		isFire_ = true;
	}
}