#include "_Debug/_DebugConOut.h"
#include "Gravity.h"
#include "ActionRect.h"
#include "Loader/CollisionLoader.h"
#include "AnimationEnum/AnimationType.h"
#include "anim/AnimMng.h"
#include "SoundMng.h"
#include "obj/Player.h"
#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"
#include "obj/Objects/Fireball.h"
#include "Cultist.h"

USING_NS_CC;

Cultist::Cultist(Vec2 pos, Player& player, BehaviorTree* aiTree, VisionRange visionRange, int hp,Layer& myLayer):
    Enemy(pos,player, aiTree, visionRange, hp,myLayer)
{
	attackCnt_ = 0;
	myName_ = "cultist";
	flag_   = false;
	fireBallCT_ = 0;

	lpSoundMng.AddSound("burst01", "BGM/s-burst01.mp3",SoundType::SE);

	this->setScale(2.0f);
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::Cultist;

	// �A�j���[�V�����̓o�^
	ActModuleRegistration();

	currentAnimation_ = "cultist_idle";
	previousAnimation_ = currentAnimation_;

	AnimRegistrator();
	this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));

	direction_ = Direction::Left;

	// �U����`�̃T�C�Y�ݒ�
	attackRect_.size_ = Size(30.0f, 30.0f);

	for (auto anim : lpAnimMng.GetAnimations(type_))
	{
		// colliderBox��Load
		lpCol.ReadData(collider_, anim);
	}

	// �����A�j���[�V�����̃Z�b�g
	lpAnimMng.InitAnimation(*this, ActorType::Cultist, "cultist_idle");
}

Cultist::~Cultist()
{
}

Cultist* Cultist::CreateCultist(Vec2 pos, Player& player, BehaviorTree* aiTree, VisionRange visionRange, int hp,Layer& myLayer)
{
	Cultist* pRet = new(std::nothrow) Cultist(pos,player, aiTree, visionRange, hp,myLayer);
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

void Cultist::update(float delta)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		animationFrame_ = 0.0f;
		return;
	}
	// ���񂾔���
	if (getName() == "cultist_death" || getName() == "changeFloor_death")
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
					ChangeAnimation("cultist_hit");
					stateTransitioner_ = &Enemy::Hit;
				}
			}
		}
		// hp��0�ɂȂ�����
		if (hp_ <= 0)
		{
			if (stateTransitioner_ != &Enemy::Death)
			{
				ChangeAnimation("cultist_death");
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
			currentAnimation_  = "cultist_idle";
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

bool Cultist::AddAttackObj(const float& angle)
{
	// �A���ōU������̂�h��
	if (flag_)
	{
		// 1�x�U��������ɍēx�U������܂ł̃N�[���^�C����ݒ肷��
		if (fireBallCT_ > 0)
		{
			fireBallCT_--;
		}
		else
		{
			fireBallCT_ = 0;
			flag_ = false;
		}
		currentAnimation_ = "cultist_idle";
		isAttacking_ = false;
		return false;
	}

	lpSoundMng.PlayBySoundName("burst01");
	flag_ = true;
	fireBallCT_ = 120;

	// �U�����Ă�����fireball���΂�
	auto fireball = Fireball::CreateFireball({ getPosition().x,getPosition().y + 30.0f },direction_,angle,player_);
	fireball->runAction(FlipX::create((direction_ == Direction::Left ? false : true)));
	// �U��ڲ԰��fireball���Ԃ牺����
	attackLayer_->addChild(fireball,0);
	attackCnt_++;
	isAttacking_ = true;
	fireball->scheduleUpdate();
	return true;
}

const float Cultist::GetPLAngle(void)
{
	const auto plPos = player_.getPosition();
	const auto angle = atan2(getPosition().y - plPos.y,getPosition().x - plPos.x);
	return angle;
}

void Cultist::ActModuleRegistration(void)
{
	Actor::XmlActDataRead("cultistModule", nullptr);
}

void Cultist::NormalAttack(void)
{
	if (animationFrame_int_ < 11)
	{
		currentCol_ = collider_[currentAnimation_][animationFrame_int_];
	}

	if (animationFrame_ >= 0.01f)
	{
		isFire_ = true;
	}
}