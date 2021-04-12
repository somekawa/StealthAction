#include "_Debug/_DebugConOut.h"
#include "Gravity.h"
#include "ActionRect.h"
#include "Loader/CollisionLoader.h"
#include "AnimationEnum/AnimationType.h"
#include "anim/AnimMng.h"
#include "obj/Player.h"
#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"
#include "Assassin.h"

USING_NS_CC;

Assassin::Assassin(Vec2 pos, Player& player,
	BehaviorTree* aiTree,VisionRange visionRange,int hp,Layer& myLayer):
	Enemy(pos,player,aiTree,visionRange,hp,myLayer)
{
	this->setScale(1.5f);
	myName_   = "assassin";
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::Assassin;
	// ActModuleの登録
	ActModuleRegistration();

	currentAnimation_ = "assassin_idle";
	previousAnimation_ = currentAnimation_;
	direction_ = Direction::Right;

	// 攻撃矩形のサイズ設定
	attackRect_.size_ = Size(50.0f, 50.0f);

	AnimRegistrator();
	for (auto anim : lpAnimMng.GetAnimations(type_))
	{
		// colliderBoxのLoad
		lpCol.ReadData(collider_, anim);
	}
	
	// 初期アニメーションのセット
	lpAnimMng.InitAnimation(*this, ActorType::Assassin, "assassin_idle");
}

Assassin::~Assassin()
{
	attackLayer_->removeFromParentAndCleanup(true);
}

Assassin* Assassin::CreateAssassin(Vec2 pos, Player& player,
	BehaviorTree* aiTree,VisionRange visionRange,int hp,Layer& myLayer)
{
	Assassin* pRet = new(std::nothrow) Assassin(pos,player,aiTree,visionRange,hp,myLayer);
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

void Assassin::update(float delta)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		animationFrame_ = 0.0f;
		animationFrame_int_ = 0;
		return;
	}

	// 死亡判定
	if (getName() == "assassin_death" || getName() == "changeFloor_death")
	{
		attackLayer_->removeFromParentAndCleanup(true);
		// 自分を親であるGameSceneから削除する
		this->removeFromParentAndCleanup(true);
	}
	else
	{
		setAnchorPoint(Vec2(0.5f,0.0f));
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
					ChangeAnimation("assassin_hit");
					stateTransitioner_ = &Enemy::Hit;
				}
			}
		}

		// hpが0になったら
		if (hp_ <= 0)
		{
			if (stateTransitioner_ != &Enemy::Death)
			{
				ChangeAnimation("assassin_death");
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
			currentAnimation_  = "assassin_idle";
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

bool Assassin::AddAttackObj(const float& angle)
{
	return true;
}

void Assassin::ActModuleRegistration(void)
{
	Actor::XmlActDataRead("assassinModule", nullptr);
}

void Assassin::NormalAttack(void)
{
	if (animationFrame_int_ < 13)
	{
		currentCol_ = collider_[currentAnimation_][animationFrame_int_];
	}
}