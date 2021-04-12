#include "_Debug/_DebugConOut.h"
#include "Loader/CollisionLoader.h"
#include "ActionRect.h"
#include "AnimationEnum/AnimationType.h"
#include "anim/AnimMng.h"
#include "obj/Player.h"
#include "Gravity.h"
#include "TwistedCultist.h"

USING_NS_CC;

TwistedCultist::TwistedCultist(Vec2 pos,Player& player,
	BehaviorTree* aiTree,VisionRange visionRange,int hp,Layer& myLayer):
	Enemy(pos,player,aiTree,visionRange,hp,myLayer)
{
	this->setScale(2.0f);
	myName_   = "twistedCultist";
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::TwistedCultist;

	// �A�j���[�V�����̓o�^
	ActModuleRegistration();

	currentAnimation_ = "twistedCultist_idle";
	direction_ = Direction::Left;

	AnimRegistrator();
	for (auto anim : lpAnimMng.GetAnimations(type_))
	{
		// colliderBox��Load
		lpCol.ReadData(collider_, anim);
	}

	// �����A�j���[�V�����̃Z�b�g
	lpAnimMng.InitAnimation(*this, ActorType::TwistedCultist, "twistedCultist_idle");
	updater_ = &TwistedCultist::Idle;
}

TwistedCultist::~TwistedCultist()
{
	attackLayer_->removeFromParentAndCleanup(true);
}

TwistedCultist* TwistedCultist::CreateTwistedCultist(Vec2 pos,Player& player, 
	BehaviorTree* aiTree, VisionRange visionRange,int hp,Layer& myLayer)
{
	TwistedCultist* pRet = new(std::nothrow) TwistedCultist(pos,player,aiTree,visionRange,hp,myLayer);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

void TwistedCultist::update(float delta)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		animationFrame_ = 0.0f;
		animationFrame_int_ = 0;
		return;
	}

	// ���S����
	if (getName() == "twistedCultist_death" || getName() == "changeFloor_death")
	{
		attackLayer_->removeFromParentAndCleanup(true);
		// ������e�ł���GameScene����폜����
		this->removeFromParentAndCleanup(true);
	}
	else
	{
		setAnchorPoint(Vec2(0.5f, 0.0f));
		if (!isAttacking_)
		{
			// �����̕ύX
			ChangeDirection(delta);
		}
		// ���݂̃t���[���𐮐��l�Ŏ擾
		animationFrame_int_ = GetAnimationFrameInt() - 1;

		// 0�ȉ��ɂȂ��0�ɂ���
		if (animationFrame_int_ < 0)
		{
			animationFrame_int_ = 0;
		}

		// ��Ұ�ނ�������Ă��Ȃ����Ǝ���Ӱ��݂łȂ��ꍇ
		if (!isHitAttack_ && stateTransitioner_ != &Enemy::Death)
		{
			// actCtl�̱����ްĂ���
			actCtl_.Update(type_, *this);
		}

		if (stateTransitioner_ != &Enemy::Death)
		{
			// ��Ұ�ނ�H�����
			if (isHitAttack_)
			{
				if (stateTransitioner_ != &Enemy::Hit)
				{
					ChangeAnimation("twistedCultist_hit");
					stateTransitioner_ = &Enemy::Hit;
				}
			}
		}

		// hp��0�ɂȂ�����
		if (hp_ <= 0)
		{
			if (stateTransitioner_ != &Enemy::Death)
			{
				ChangeAnimation("twistedCultist_death");
				stateTransitioner_ = &Enemy::Death;
			}
		}

		// �A�j���[�V�����̍X�V
		UpdateAnimation(delta);
		// ��Ԃ̑J��
		(this->*stateTransitioner_)();

		// �A�j���[�V�����I�����ɍU���t���O��false
		if (isAnimEnd_)
		{
			isHitAttack_ = false;
			isAttacking_ = false;
			hittingToPlayer_ = false;
			stateTransitioner_ = &Enemy::Idle;
			currentAnimation_ = "twistedCultist_idle";
		}

		if (currentAnimation_ != previousAnimation_)
		{
			ChangeAnimation(currentAnimation_);
		}
		previousAnimation_ = currentAnimation_;
		// �e��`���̾��
		SetCollider();
	}
}

bool TwistedCultist::AddAttackObj(const float& angle)
{
	return true;
}

void TwistedCultist::ActModuleRegistration(void)
{
	Actor::XmlActDataRead("twistedcultistModule", nullptr);
}

void TwistedCultist::NormalAttack(void)
{
	if (animationFrame_int_ < 7)
	{
		currentCol_ = collider_[currentAnimation_][animationFrame_int_];
	}
	if (OnAttacked())
	{
		player_.OnIsHitAttack();
	}
}