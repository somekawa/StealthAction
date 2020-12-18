#include "TwistedCultist.h"
#include "_Debug/_DebugConOut.h"
#include "Loader/CollisionLoader.h"
#include "anim/AnimMng.h"

USING_NS_CC;

TwistedCultist::TwistedCultist(Player& player,
	BehaviorTree* aiTree,VisionRange visionRange,int hp):
	Enemy(player,aiTree,visionRange,hp)
{
	pos_ = { 980,200 };
	this->setPosition(Vec2(pos_.x, pos_.y));
	this->setScale(2.0f);
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::TwistedCultist;

	// �A�j���[�V�����̓o�^
	AnimRegistrator();

	currentAnimation_ = "idle";
	this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
	direction_ = Direction::Left;

	lpCol.Load(collider_, "idle", "twistedCultist");

	updater_ = &TwistedCultist::Idle;
}

TwistedCultist::~TwistedCultist()
{
}

TwistedCultist* TwistedCultist::CreateTwistedCultist(Player& player, 
	BehaviorTree* aiTree, VisionRange visionRange,int hp)
{
	TwistedCultist* pRet = new(std::nothrow) TwistedCultist(player,aiTree,visionRange,hp);
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

void TwistedCultist::Action(void)
{
	ChangeDirection();
	(this->*updater_)();
}

void TwistedCultist::update(float delta)
{
	Action();
	// ���݂̃t���[���𐮐��l�Ŏ擾
	animationFrame_int_ = GetAnimationFrameInt();
	// �A�j���[�V�����̍X�V
	UpdateAnimation(delta);
}

void TwistedCultist::AnimRegistrator(void)
{
	// idle
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/twistedCultist/twistedCultist", "idle", 6, 0.3f, ActorType::TwistedCultist, true);
	// walk
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/twistedCultist/twistedCultist", "walk", 8, 0.3f, ActorType::TwistedCultist, true);
	// death
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/twistedCultist/twistedCultist", "death", 12, 0.3f, ActorType::TwistedCultist, true);

}

void TwistedCultist::NormalAttack(void)
{
}

void TwistedCultist::UseSkill(void)
{
}

void TwistedCultist::Patrol(void)
{
}

void TwistedCultist::Chase(void)
{
}

void TwistedCultist::Run(void)
{
}

void TwistedCultist::Jump(void)
{
}

void TwistedCultist::Fall(void)
{
}
