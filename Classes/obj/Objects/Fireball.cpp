#include "Fireball.h"
#include "anim/AnimMng.h"

USING_NS_CC;

Fireball::Fireball(Vec2 pos,Direction direction,float angle,Actor& target):
	Obj(target)
{
	setPosition(pos);
	type_ = ActorType::Fireball;
	AnimRegistrator();
	currentAnimation_ = "normal";

	if (direction == Direction::Left)
	{
		dirFlag_ = false;
		speed_ = { -2.0f,0.0f };
	}
	if (direction == Direction::Right)
	{
		dirFlag_ = true;
		speed_ = { 2.0f,0.0f };
	}
	flipFlag_ = FlipX::create(dirFlag_);

	this->runAction(flipFlag_);
}

Fireball::~Fireball()
{
}

void Fireball::update(float delta)
{
	Move();

	if (currentAnimation_ != "impact")
	{
		if (HitToTarget())
		{
			ChangeAnimation("impact");
		}
	}

	if (isHitTarget_)
	{
		if (isAnimEnd_)
		{
			//removeFromParentAndCleanup(true);
		}
	}

	UpdateAnimation(delta);
}

void Fireball::Move(void)
{
	auto move = MoveTo::create(0.0f, Vec2(getPosition().x+speed_.x, getPosition().y + speed_.y));
	this->runAction(move);
}

void Fireball::AnimRegistrator(void)
{


	lpAnimMng.InitAnimation(*this, ActorType::Fireball, "normal");
}

Fireball* Fireball::CreateFireball(Vec2 pos,Direction direction,float angle,Actor& target)
{
	Fireball* pRet = new(std::nothrow) Fireball(pos,direction,angle,target);
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
