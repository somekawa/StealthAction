#include "Assassin.h"
#include "_Debug/_DebugConOut.h"
#include "anim/AnimMng.h"

USING_NS_CC;

Assassin::Assassin(cocos2d::Vector<cocos2d::Node*>& player):
	Enemy(player)
{
	pos_ = { 980,200 };
	this->setPosition(Vec2(pos_.x, pos_.y));
	this->setScale(2.0f);
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::Assassin;

	// �A�j���[�V�����̓o�^
	AnimRegistrator();

	currentAnimation_ = "idle";
	this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
	direction_ = Direction::Left;

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
	Action();
	// �A�j���[�V�����̍X�V
	UpdateAnimation(delta);
	//TRACE("Enemy->%s", currentAnimation_);
}

void Assassin::AnimRegistrator(void)
{
	// �A�j���[�V�������L���b�V���ɓo�^
	// idle
	// ��������ύX�����ق����悢
	// �p�X������������ł��ď����Ӗ��Ȃ� ex.)image/EnemyAnimationAsset/assassin�ł���
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "idle", 8, 0.3f, ActorType::Assassin, true);
	// run
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "run", 8, 0.08f, ActorType::Assassin, true);

	// attack
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "attack", 13, 0.08f, ActorType::Assassin, false);

	// death
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "death", 16, 1.0f, ActorType::Assassin, false);
	// �����A�j���[�V�����̃Z�b�g
	lpAnimMng.InitAnimation(*this, ActorType::Assassin, "idle");
}

void Assassin::Idle(void)
{
	// 20.0f�̓}�W�b�N�i���o�[
	// ��ŕύX����
	if (DistanceCalcurator() < 200.0f)
	{
		// �A�j���[�V������ύX����
		ChangeAnimation("run");
		// ���̕ύX�����A�j���[�V�����𑖂点��
		this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
		// �s����run�ɂ���
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

	// ���̂Ƃ�50.0f�̓}�W�b�N�i���o�[
	// ��ŕύX����
	if (DistanceCalcurator() <= 50.0f)
	{
		// �A�j���[�V������ύX����
		ChangeAnimation("attack");
		// �ύX�����A�j���[�V�����𑖂点��
		this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
		// �s����attack�ɂ���
		updater_ = &Assassin::Attack;
	}
}

void Assassin::Attack(void)
{
	// attack���[�V�������I��������
	if (isAnimEnd_)
	{
		// �A�j���[�V������ύX����
		ChangeAnimation("idle");
		// �ύX�����A�j���[�V�����𑖂点��
		this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
		// �s����idle�ɂ���
		updater_ = &Assassin::Idle;
	}
}
