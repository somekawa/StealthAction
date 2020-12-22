#include "Cultist.h"
#include "Gravity.h"
#include "_Debug/_DebugConOut.h"
#include "ActionRect.h"
#include "Loader/CollisionLoader.h"
#include "anim/AnimMng.h"
#include "obj/Player.h"

#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"

#include "obj/Objects/Fireball.h"

USING_NS_CC;

Cultist::Cultist(Player& player, BehaviorTree* aiTree, VisionRange visionRange, int hp):
    Enemy(player, aiTree, visionRange, hp)
{
	pos_ = { 500,500 };
	this->setPosition(Vec2(pos_.x, pos_.y));
	this->setScale(1.5f);
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::Cultist;

	// アニメーションの登録
	AnimRegistrator();
	// ActModuleの登録
	//ActModuleRegistrator();

	currentAnimation_ = "idle";
	previousAnimation_ = currentAnimation_;

	this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));

	direction_ = Direction::Left;

	//lpCol.Load(collider_, "idle", "assassin");
	// 攻撃矩形のサイズ設定
	attackRect_.size_ = Size(30.0f, 30.0f);
	//for (auto col : collider_["idle"])
	//{
	//	for (int colNum = 0; colNum < col.size(); colNum++)
	//	{
	//		// colliderBoxを自身の子にする
	//		auto draw = col[colNum]->create();
	//		// ポジションがおかしい...
	//		draw->setAnchorPoint(Vec2(0.0f, 0.0f));
	//		draw->setContentSize(Size{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y });
	//		draw->drawRect(Vec2(0, 0), Vec2{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y }, col[colNum]->GetColor());
	//		draw->setTag(col[colNum]->GetData().frame_);
	//		this->addChild(draw, 0, "idle");
	//	}
	//}

	for (auto anim : lpAnimMng.GetAnimations(type_))
	{
		// colliderBoxのLoad
		lpCol.Load(collider_, anim, "cultist");
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

Cultist::~Cultist()
{
}

Cultist* Cultist::CreateCultist(Player& player, BehaviorTree* aiTree, VisionRange visionRange, int hp)
{
	Cultist* pRet = new(std::nothrow) Cultist(player, aiTree, visionRange, hp);
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

void Cultist::Action(void)
{
}

void Cultist::update(float delta)
{
	// 死んだ判定
	if (getName() == "death")
	{
		// 自分を親であるGameSceneから削除する
		removeFromParentAndCleanup(true);
	}
	else
	{
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
			currentCol_ = collider_[currentAnimation_][animationFrame_int_];
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

void Cultist::AnimRegistrator(void)
{
	// アニメーションをキャッシュに登録
	// idle
	// 第一引数を変更したほうがよい
	// パスが同名が並んでいて書く意味なし ex.)image/EnemyAnimationAsset/assassinでいい
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "idle", 6, 0.03f, ActorType::Cultist, true);
	// run
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "walk", 8, 0.08f, ActorType::Cultist, true);

	// attack
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "attack", 10, 0.08f, ActorType::Cultist, false);

	// death
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "death", 12, 0.08f, ActorType::Cultist, false);

	// hit 
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "hit", 3, 0.09f, ActorType::Cultist, false);
	// 初期アニメーションのセット
	lpAnimMng.InitAnimation(*this, ActorType::Cultist, "idle");
}

void Cultist::AddAttackObj(void)
{
	auto fireball = Fireball::CreateFireball(getPosition(), type_);

	this->addChild(fireball);

	fireball->scheduleUpdate();
}

void Cultist::NormalAttack(void)
{
	isAttacking_ = true;


}

void Cultist::UseSkill(void)
{
}

void Cultist::Patrol(void)
{
}

void Cultist::Chase(void)
{
}

void Cultist::Run(void)
{
}

void Cultist::Jump(void)
{
}

void Cultist::Fall(void)
{
}
