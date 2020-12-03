#include "Assassin.h"
#include "Gravity.h"
#include "_Debug/_DebugConOut.h"
#include "ActionRect.h"
#include "Loader/CollisionLoader.h"
#include "anim/AnimMng.h"

#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"

USING_NS_CC;

Assassin::Assassin(cocos2d::Vector<cocos2d::Node*>& player, BehaviorTree* aiTree):
	Enemy(player,aiTree)
{
	pos_ = { 500,500 };
	this->setPosition(Vec2(pos_.x, pos_.y));
	this->setScale(2.0f);
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::Assassin;

	// アニメーションの登録
	AnimRegistrator();
	// ActModuleの登録
	//ActModuleRegistrator();

	currentAnimation_ = "idle";
	previousAnimation_ = currentAnimation_;

	this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_,currentAnimation_)));

	direction_ = Direction::Left;

	lpCol.Load(collider_, "idle", "assassin");

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
		lpCol.Load(collider_, anim, "assassin");
		for (auto col : collider_[anim])
		{
			for (int colNum = 0; colNum < col.size(); colNum++)
			{
				// colliderBoxを自身の子にする
				auto draw = col[colNum]->create();
				draw->setContentSize(Size{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y });
				draw->drawRect(Vec2(0, 0), Vec2{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y }, col[colNum]->GetColor());
				draw->setTag(col[colNum]->GetData().frame_);
				this->addChild(draw, 0, anim);
			}
		}
	}

	//updater_ = &Assassin::Idle;
}

Assassin::~Assassin()
{

}

Assassin* Assassin::CreateAssassin(cocos2d::Vector<cocos2d::Node*>& player, BehaviorTree* aiTree)
{
	Assassin* pRet = new(std::nothrow) Assassin(player,aiTree);
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

void Assassin::Action(void)
{
	//(this->*updater_)();
}

void Assassin::update(float delta)
{
	if (!isAttacking_)
	{
		// 方向の変更
		ChangeDirection();
	}
	//previousAnimation_ = currentAnimation_;
	// AI起動
	AIRun();
	TRACE("attackFlag:%d\n", isAttacking_);
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		animationFrame_ = 0.0f;
		return;
	}
	// 重力をかける
	gravity_->ApplyGravityToTarget(delta);
	Action();

	// 現在のフレームを整数値で取得
	animationFrame_int_ = GetAnimationFrameInt();
	for (auto animationCol = this->getChildren().rbegin();
		animationCol != this->getChildren().rend(); animationCol++)
	{
		TRACE("animationInt:%d\n", animationFrame_int_);
		auto i = (*animationCol)->getTag()-2;
		TRACE("i:%d,animInt:%d\n", i,animationFrame_int_);
		if (currentAnimation_ == (*animationCol)->getName() &&
			animationFrame_int_==i)
		{
			(*animationCol)->setVisible(true);
		}
		else
		{
			(*animationCol)->setVisible(false);
		}
	}
	// コライダーの描画
	//for (auto collider : this->getChildren())
	//{
	//	if (currentAnimation_ == collider->getName()/* &&
	//		animationFrame_int_ == collider->getTag()*/)
	//	{
	//		collider->setVisible(true);
	//	}
	//	else
	//	{
	//		collider->setVisible(false);
	//	}
	//}
	// アニメーションの更新
	UpdateAnimation(delta);
	// アニメーション終了時に攻撃フラグをfalse
	if (isAnimEnd_)
	{
		isAttacking_ = false;
	}
}

void Assassin::AnimRegistrator(void)
{
	// アニメーションをキャッシュに登録
	// idle
	// 第一引数を変更したほうがよい
	// パスが同名が並んでいて書く意味なし ex.)image/EnemyAnimationAsset/assassinでいい
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "idle", 8, 0.03f, ActorType::Assassin, true);
	// run
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "run", 8, 0.08f, ActorType::Assassin, true);

	// attack
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "attack", 13, 0.08f, ActorType::Assassin, false);

	// death
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "death", 16, 1.0f, ActorType::Assassin, false);
	// 初期アニメーションのセット
	lpAnimMng.InitAnimation(*this, ActorType::Assassin, "idle");
}

void Assassin::Jump(void)
{
}

void Assassin::Fall(void)
{
}

void Assassin::Hit(void)
{
}

void Assassin::Death(void)
{
}

void Assassin::NormalAttack(void)
{
	isAttacking_ = true;
	//TRACE("attackFlag:%d\n", isAttacking_);
	//currentAnimation_ = "attack";
	if (isAnimEnd_)
	{
		isAttacking_ = false;
		//TRACE("attackFlag:%d\n", isAttacking_);

		ChangeAnimation("idle");
	}
}

void Assassin::UseSkill(void)
{
}

void Assassin::Idle(void)
{

}

void Assassin::Patrol(void)
{
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
	currentAnimation_ = "run";
	pos_.x += speed_.x;
	this->setPosition(Vec2(pos_.x, pos_.y));
	this->runAction(flipFlag_);
}

void Assassin::Chase(void)
{
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
	currentAnimation_ = "run";
	pos_.x += speed_.x;
	this->setPosition(Vec2(pos_.x, pos_.y));
	this->runAction(flipFlag_);
}

void Assassin::Run(void)
{
	switch (direction_)
	{
	case Direction::Right:
		speed_.x = 4;
		flipFlag_ = FlipX::create(true);
		break;
	case Direction::Left:
		speed_.x = -4;
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

	pos_.x += speed_.x;
	this->setPosition(Vec2(pos_.x, pos_.y));
	this->runAction(flipFlag_);

	// 今のとこ50.0fはマジックナンバー
	// 後で変更あり
	if (DistanceCalcurator() < 50.0f)
	{
		// アニメーションを変更して
		ChangeAnimation("attack");
		// 変更したアニメーションを走らせる
		this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
		// 行動をattackにする
		updater_ = &Assassin::Attack;
	}
}

void Assassin::Attack(void)
{
	// attackモーションが終了したら
	if (isAnimEnd_)
	{
		// アニメーションを変更して
		ChangeAnimation("idle");
		// 変更したアニメーションを走らせる
		this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
		// 行動をidleにする
		updater_ = &Assassin::Idle;
	}
}
