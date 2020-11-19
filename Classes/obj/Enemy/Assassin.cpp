#include "Assassin.h"
#include "Gravity.h"
#include "_Debug/_DebugConOut.h"
#include "ActionRect.h"
#include "Loader/CollisionLoader.h"
#include "anim/AnimMng.h"

USING_NS_CC;

Assassin::Assassin(cocos2d::Vector<cocos2d::Node*>& player):
	Enemy(player)
{
	pos_ = { 980,500 };
	this->setPosition(Vec2(pos_.x, pos_.y));
	this->setScale(2.0f);
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::Assassin;

	// アニメーションの登録
	AnimRegistrator();

	currentAnimation_ = "idle";
	this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
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

	updater_ = &Assassin::Idle;

}

Assassin::~Assassin()
{
}

Assassin* Assassin::CreateAssassin(cocos2d::Vector<cocos2d::Node*>& player)
{
	Assassin* pRet = new(std::nothrow) Assassin(player);
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
	ChangeDirection();
	(this->*updater_)();
}

void Assassin::update(float delta)
{
	gravity_->ApplyGravityToTarget(delta);

	Action();
	// 現在のフレームを整数値で取得
	animationFrame_int_ = GetAnimationFrameInt();
	for (auto collider : this->getChildren())
	{
		if (currentAnimation_ == collider->getName() &&
			animationFrame_int_ == collider->getTag())
		{
			collider->setVisible(true);
		}
		else
		{
			collider->setVisible(false);
		}
	}
	// アニメーションの更新
	UpdateAnimation(delta);
	//TRACE("Enemy->%s", currentAnimation_);
}

void Assassin::AnimRegistrator(void)
{
	// アニメーションをキャッシュに登録
	// idle
	// 第一引数を変更したほうがよい
	// パスが同名が並んでいて書く意味なし ex.)image/EnemyAnimationAsset/assassinでいい
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "idle", 8, 0.3f, ActorType::Assassin, true);
	// run
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "run", 8, 0.08f, ActorType::Assassin, true);

	// attack
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "attack", 13, 0.08f, ActorType::Assassin, false);

	// death
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "death", 16, 1.0f, ActorType::Assassin, false);
	// 初期アニメーションのセット
	lpAnimMng.InitAnimation(*this, ActorType::Assassin, "idle");
}

void Assassin::Idle(void)
{
	// 20.0fはマジックナンバー
	// 後で変更あり
	if (DistanceCalcurator() < 200.0f)
	{
		// アニメーションを変更して
		ChangeAnimation("run");
		// その変更したアニメーションを走らせる
		this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
		// 行動をrunにする
		updater_ = &Assassin::Run;
	}
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
	if (DistanceCalcurator() <= 50.0f)
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
