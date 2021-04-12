#include "_Debug/_DebugConOut.h"
#include "Loader/CollisionLoader.h"
#include "ActionRect.h"
#include "AnimationEnum/AnimationType.h"
#include "anim/AnimMng.h"
#include "obj/Player.h"
#include "Gravity.h"
#include "TwistedCultist.h"

USING_NS_CC;

TwistedCultist::TwistedCultist(Vec2 pos,Player& player,
	BehaviorTree* aiTree,VisionRange visionRange,int hp,Layer& myLayer):
	Enemy(pos,player,aiTree,visionRange,hp,myLayer)
{
	this->setScale(2.0f);
	myName_   = "twistedCultist";
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::TwistedCultist;

	// アニメーションの登録
	ActModuleRegistration();

	currentAnimation_ = "twistedCultist_idle";
	direction_ = Direction::Left;

	AnimRegistrator();
	for (auto anim : lpAnimMng.GetAnimations(type_))
	{
		// colliderBoxのLoad
		lpCol.ReadData(collider_, anim);
	}

	// 初期アニメーションのセット
	lpAnimMng.InitAnimation(*this, ActorType::TwistedCultist, "twistedCultist_idle");
	updater_ = &TwistedCultist::Idle;
}

TwistedCultist::~TwistedCultist()
{
	attackLayer_->removeFromParentAndCleanup(true);
}

TwistedCultist* TwistedCultist::CreateTwistedCultist(Vec2 pos,Player& player, 
	BehaviorTree* aiTree, VisionRange visionRange,int hp,Layer& myLayer)
{
	TwistedCultist* pRet = new(std::nothrow) TwistedCultist(pos,player,aiTree,visionRange,hp,myLayer);
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

void TwistedCultist::update(float delta)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		animationFrame_ = 0.0f;
		animationFrame_int_ = 0;
		return;
	}

	// 死亡判定
	if (getName() == "twistedCultist_death" || getName() == "changeFloor_death")
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
					ChangeAnimation("twistedCultist_hit");
					stateTransitioner_ = &Enemy::Hit;
				}
			}
		}

		// hpが0になったら
		if (hp_ <= 0)
		{
			if (stateTransitioner_ != &Enemy::Death)
			{
				ChangeAnimation("twistedCultist_death");
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
			currentAnimation_ = "twistedCultist_idle";
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

bool TwistedCultist::AddAttackObj(const float& angle)
{
	return true;
}

void TwistedCultist::ActModuleRegistration(void)
{
	Actor::XmlActDataRead("twistedcultistModule", nullptr);
}

void TwistedCultist::NormalAttack(void)
{
	if (animationFrame_int_ < 7)
	{
		currentCol_ = collider_[currentAnimation_][animationFrame_int_];
	}
	if (OnAttacked())
	{
		player_.OnIsHitAttack();
	}
}