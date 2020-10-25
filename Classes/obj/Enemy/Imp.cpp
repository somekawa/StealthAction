#include "Imp.h"
#include "anim/AnimMng.h"

USING_NS_CC;

// Ž‹ŠE
constexpr int Sight = 150;
constexpr int AttackRange = 45;

Imp::Imp(Vector<Node*>& player, std::unordered_map<std::string,
	std::vector<std::vector<std::shared_ptr<ActionRect>>>>&collider):
	Enemy(player,collider)
{
	pos_ = { 980,100 };
	this->setPosition(Vec2(pos_.x, pos_.y));
	this->setScale(2.0f);
	//sprite_->setTextureRect(rect);
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::Imp;

	lpAnimMng.InitAnimation(*this, type_);
	//lpAnimMng.InitAnimation(*this,type_,"walk");

	direction_ = Direction::Left;

	updater_ = &Imp::Walk;
}

Imp::~Imp()
{
}

Imp* Imp::CreateImp(Vector<Node*>& player, std::unordered_map<std::string,
	std::vector<std::vector<std::shared_ptr<ActionRect>>>>&collider)
{
	Imp* pRet = new(std::nothrow) Imp(player,collider);
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

	if (DistanceCalcurator() <= Sight)
	{
		currentAnimation_ = "run";
		lpAnimMng.ChangeAnimation(*this, "run", true,type_);
		updater_ = &Imp::Run;
	}

	if (pos_.x <= 0)
	{
		currentAnimation_ = "death";
		lpAnimMng.ChangeAnimation(*this, "death", false,type_);
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
		currentAnimation_ = "attack1";
		lpAnimMng.ChangeAnimation(*this, "attack1", true,type_);
		updater_ = &Imp::Attack;
	}

	if (DistanceCalcurator() >= Sight)
	{
		currentAnimation_ = "walk";
		lpAnimMng.ChangeAnimation(*this, "walk", true,type_);
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
		lpAnimMng.ChangeAnimation(*this, "walk", true, type_);
		updater_ = &Imp::Walk;
	}
}

void Imp::Death(void)
{
	animationFrame_ += 0.02f;
	if (lpAnimMng.IsAnimEnd(animationFrame_,type_,currentAnimation_))
	{
		this->setName("death");
		animationFrame_ = lpAnimMng.GetAnimationMaxFrame(type_,currentAnimation_);
		Delete();
	}
}
