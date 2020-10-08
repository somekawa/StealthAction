#include "Imp.h"
#include "Collision/Collider.h"
#include "anim/AnimMng.h"

USING_NS_CC;

// Ž‹ŠE
constexpr int Sight = 150;
constexpr int AttackRange = 45;

Imp::Imp(std::vector<std::shared_ptr<Player>>& player):
	Enemy(player)
{
	pos_ = Vec2(980, 0);
	auto rect = Rect(0, 0, 20, 10);
	sprite_ = Sprite::create(/*"imp_walk.png"*/);
	//sprite_->setTextureRect(rect);
	sprite_->setPosition(pos_);
	sprite_->setAnchorPoint(Vec2(0.5, 0.0f));
	sprite_->setScale(2.0f);
	sprite_->setName("Imp");
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::Imp;
	// ŽŽ‚µ‚É‘‚¢‚Ä‚Ý‚½
	
	//auto drawRect = DrawNode::create();
	//drawRect->drawRect(collider_->GetPos(),collider_->GetPos() + collider_->GetSize(),Color4F::RED);
	//sprite_->addChild(drawRect);
	lpAnimMng.InitAnimation(*sprite_,type_);
	collider_->Create(*sprite_);
	direction_ = Direction::Left;

	updater_ = &Imp::Walk;
}

Imp::~Imp()
{
}

Imp* Imp::CreateImp(std::vector<std::shared_ptr<Player>>& player)
{
	//return Imp::create();

	Imp* pRet = new(std::nothrow) Imp(player);
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

void Imp::Action(void)
{
	ChangeDirection();

	//AnimationManager::ChangeAnimation(sprite_, "run");
	(this->*updater_)();
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
	sprite_->setPosition(pos_);
	sprite_->runAction(flipFlag_);

	if (DistanceCalcurator() <= Sight)
	{
		currentAnimation_ = "run";
		lpAnimMng.ChangeAnimation(*sprite_, "run", true,type_);
		updater_ = &Imp::Run;
	}

	if (pos_.x <= 0)
	{
		currentAnimation_ = "death";
		lpAnimMng.ChangeAnimation(*sprite_, "death", false,type_);
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
	sprite_->setPosition(pos_);
	sprite_->runAction(flipFlag_);

	if (DistanceCalcurator() <= AttackRange)
	{
		currentAnimation_ = "attack1";
		lpAnimMng.ChangeAnimation(*sprite_, "attack1", true,type_);
		updater_ = &Imp::Attack;
	}

	if (DistanceCalcurator() >= Sight)
	{
		currentAnimation_ = "walk";
		lpAnimMng.ChangeAnimation(*sprite_, "walk", true,type_);
		updater_ = &Imp::Walk;
	}
}

void Imp::Attack(void)
{
	animationFrame_ += 0.01f;
	if (lpAnimMng.IsAnimEnd(animationFrame_, type_, currentAnimation_))
	{
		animationFrame_ = 0.0f;
		currentAnimation_ = "walk";
		lpAnimMng.ChangeAnimation(*sprite_, "walk", true, type_);
		updater_ = &Imp::Walk;
	}
}

void Imp::Death(void)
{
	animationFrame_ += 0.01f;
	if (lpAnimMng.IsAnimEnd(animationFrame_,type_,currentAnimation_))
	{
		sprite_->setName("death");
		animationFrame_ = 0.0f;
		Delete();
	}
}
