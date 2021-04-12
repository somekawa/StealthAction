#include "_Debug/_DebugConOut.h"
#include "Gravity.h"
#include "ActionRect.h"
#include "Loader/CollisionLoader.h"
#include "AnimationEnum/AnimationType.h"
#include "anim/AnimMng.h"
#include "obj/Player.h"
#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"
#include "Assassin.h"

USING_NS_CC;

Assassin::Assassin(Vec2 pos, Player& player,
	BehaviorTree* aiTree,VisionRange visionRange,int hp,Layer& myLayer):
	Enemy(pos,player,aiTree,visionRange,hp,myLayer)
{
	this->setScale(1.5f);
	myName_   = "assassin";
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::Assassin;
	// ActModule�̓o�^
	ActModuleRegistration();

	currentAnimation_ = "assassin_idle";
	previousAnimation_ = currentAnimation_;
	direction_ = Direction::Right;

	// �U����`�̃T�C�Y�ݒ�
	attackRect_.size_ = Size(50.0f, 50.0f);

	AnimRegistrator();
	for (auto anim : lpAnimMng.GetAnimations(type_))
	{
		// colliderBox��Load
		lpCol.ReadData(collider_, anim);
	}
	
	// �����A�j���[�V�����̃Z�b�g
	lpAnimMng.InitAnimation(*this, ActorType::Assassin, "assassin_idle");
}

Assassin::~Assassin()
{
	attackLayer_->removeFromParentAndCleanup(true);
}

Assassin* Assassin::CreateAssassin(Vec2 pos, Player& player,
	BehaviorTree* aiTree,VisionRange visionRange,int hp,Layer& myLayer)
{
	Assassin* pRet = new(std::nothrow) Assassin(pos,player,aiTree,visionRange,hp,myLayer);
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

void Assassin::update(float delta)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		animationFrame_ = 0.0f;
		animationFrame_int_ = 0;
		return;
	}

	// ���S����
	if (getName() == "assassin_death" || getName() == "changeFloor_death")
	{
		attackLayer_->removeFromParentAndCleanup(true);
		// ������e�ł���GameScene����폜����
		this->removeFromParentAndCleanup(true);
	}
	else
	{
		setAnchorPoint(Vec2(0.5f,0.0f));
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
					ChangeAnimation("assassin_hit");
					stateTransitioner_ = &Enemy::Hit;
				}
			}
		}

		// hp��0�ɂȂ�����
		if (hp_ <= 0)
		{
			if (stateTransitioner_ != &Enemy::Death)
			{
				ChangeAnimation("assassin_death");
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
			currentAnimation_  = "assassin_idle";
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

bool Assassin::AddAttackObj(const float& angle)
{
	return true;
}

void Assassin::ActModuleRegistration(void)
{
	Actor::XmlActDataRead("assassinModule", nullptr);
}

void Assassin::NormalAttack(void)
{
	if (animationFrame_int_ < 13)
	{
		currentCol_ = collider_[currentAnimation_][animationFrame_int_];
	}
}