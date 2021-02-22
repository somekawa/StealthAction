#include "Fireball.h"
#include "anim/AnimMng.h"

USING_NS_CC;

Fireball::Fireball(Vec2 pos,Direction direction,float angle,Actor& target):
	Obj(target)
{
	setPosition(pos);
	AnimRegistrator();
	currentAnimation_ = "non";

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

	this->runAction(FlipX::create(dirFlag_));

	ChangeAnimation("fireball_normal");

}

Fireball::~Fireball()
{
}

void Fireball::update(float delta)
{
	if (!isHitTarget_)
	{
		Move();
	}

	if (currentAnimation_ != "fireball_impact")
	{
		if (HitToTarget())
		{
			ChangeAnimation("fireball_impact");
		}
	}

	// ファイアーボールのバグ防ぐ処理
	animationFrame_ += delta;
	if (animationFrame_ >= 0.4f && currentAnimation_ == "fireball_impact")
	{
		isAnimEnd_ = true;
	}

	UpdateAnimation(delta);
	if (isHitTarget_)
	{
		if (isAnimEnd_)
		{
			removeFromParentAndCleanup(true);
		}
	}
}

void Fireball::Move(void)
{
	auto move = MoveBy::create(0.0f, speed_);
	this->runAction(move);
}

void Fireball::AnimRegistrator(void)
{


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
