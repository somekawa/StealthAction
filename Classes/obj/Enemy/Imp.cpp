#include "Imp.h"
#include "ActionRect.h"
#include "anim/AnimMng.h"
#include "Loader/CollisionLoader.h"

USING_NS_CC;

// 視界
constexpr int Sight = 150;
constexpr int AttackRange = 45;

Imp::Imp(Player& player, BehaviorTree* aiTree, VisionRange visionRange,int hp):
	Enemy(player,aiTree,visionRange,hp)
{
	pos_ = { 980,100 };
	this->setPosition(Vec2(pos_.x, pos_.y));
	this->setScale(2.0f);
	//sprite_->setTextureRect(rect);
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::Imp;

	AnimRegistrator();
	for (auto anim : lpAnimMng.GetAnimations(type_))
	{
		// colliderBoxのLoad
		lpCol.Load(collider_, anim, "imp");
		for (auto col : collider_[anim])
		{
			for (int colNum = 0; colNum < col.size(); colNum++)
			{
				// colliderBoxを自身の子にする
				auto draw = col[colNum]->create();
				draw->setContentSize(Size{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y });
				draw->drawRect(Vec2(0, 0), Vec2{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y }, col[colNum]->GetColor());
				draw->setTag(colNum);
				this->addChild(draw, 0, anim);
			}
		}
	}
	//lpAnimMng.InitAnimation(*this,type_,"walk");
	currentAnimation_ = "walk";
	direction_ = Direction::Left;

	updater_ = &Imp::Walk;
}

Imp::~Imp()
{
}

Imp* Imp::CreateImp(Player& player, BehaviorTree* aiTree, VisionRange visionRange,int hp)
{
	Imp* pRet = new(std::nothrow) Imp(player,aiTree,visionRange,hp);
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

//Imp* Imp::CreateImp(std::vector<std::shared_ptr<Player>>& player)
//{
//	//return Imp::create();
//
//	Imp* pRet = new(std::nothrow) Imp(player);
//		if (pRet && pRet->init())
//		{
//			pRet->autorelease();
//			return pRet;
//		}
//		else
//		{
//			delete pRet;
//			pRet = nullptr;
//			return nullptr;
//		}
//}

void Imp::Action(void)
{
	ChangeDirection();
	//AnimationManager::ChangeAnimation(sprite_, "run");
	(this->*updater_)();
}

void Imp::update(float delta)
{
	Action();
	// アニメーションの更新
	UpdateAnimation(delta);
	//animationFrame_ += delta;
	// アニメーションのフレームにあった矩形を可視化する
	// ここは改良する予定(多分Actorに持っていくかも...)
	for (auto collider : this->getChildren())
	{
		if (currentAnimation_ == collider->getName())
		{
			collider->setVisible(true);
		}
		else
		{
			collider->setVisible(false);
		}
	}
}

void Imp::AnimRegistrator(void)
{
	// アニメーションをキャッシュに登録
	// walk
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/imp/imp", "walk", 6,0.3f, ActorType::Imp,true);

	// run
	//lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/imp/imp", "run", 6, (float)0.3, ActorType::Imp);

	// attack1
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/imp/imp", "attackFirst", 6,0.08f, ActorType::Imp,false);

	// death
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/imp/imp", "death", 5,1.0f, ActorType::Imp,false);

	lpAnimMng.InitAnimation(*this, ActorType::Imp, "walk");
}

void Imp::AddAttackObj(void)
{
}

void Imp::NormalAttack(void)
{
}

void Imp::Patrol(void)
{
}

void Imp::Chase(void)
{
}

void Imp::Walk(void)
{
	switch (direction_)
	{
	case Direction::Right:
		speed_.x = 3;
		flipFlag_ = FlipX::create(false);
		break;
	case Direction::Left:
		speed_.x = -3;
		flipFlag_ = FlipX::create(true);
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

	/*if (DistanceCalcurator() <= Sight)
	{
		currentAnimation_ = "run";
		lpAnimMng.ChangeAnimation(*this, "run", true,type_);
		updater_ = &Imp::Run;
	}*/

	if (pos_.x <= 0)
	{
		ChangeAnimation("death");
		//lpAnimMng.ChangeAnimation(*this, "death", false,type_);
		updater_ = &Imp::Death;
	}
}

void Imp::Run(void)
{
	switch (direction_)
	{
	case Direction::Right:
		speed_.x = 4;
		flipFlag_ = FlipX::create(false);
		break;
	case Direction::Left:
		speed_.x = -4;
		flipFlag_ = FlipX::create(true);
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

	if (DistanceCalcurator() <= AttackRange)
	{
		ChangeAnimation("attackFirst");
		//lpAnimMng.ChangeAnimation(*this, "attackFirst", true,type_);
		updater_ = &Imp::Attack;
	}

	if (DistanceCalcurator() >= Sight)
	{
		ChangeAnimation("walk");
		//lpAnimMng.ChangeAnimation(*this, "walk", true,type_);
		updater_ = &Imp::Walk;
	}
}

void Imp::Jump(void)
{
}

void Imp::Fall(void)
{
}

void Imp::UseSkill(void)
{
}

void Imp::Attack(void)
{
	//animationFrame_ += 0.01f;
	if (isAnimEnd_)
	{
		ChangeAnimation("walk");
		//lpAnimMng.ChangeAnimation(*this, "walk", true, type_);
		updater_ = &Imp::Walk;
	}
}
