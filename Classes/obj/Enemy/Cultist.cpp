#include "Cultist.h"
#include "Gravity.h"
#include "_Debug/_DebugConOut.h"
#include "ActionRect.h"
#include "Loader/CollisionLoader.h"
#include "anim/AnimMng.h"
#include "obj/Player.h"

#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"

//#include "obj/Objects/Fireball.h"

USING_NS_CC;

Cultist::Cultist(Player& player, BehaviorTree* aiTree, VisionRange visionRange, int hp):
    Enemy(player, aiTree, visionRange, hp)
{
	pos_ = { 500,500 };
	this->setPosition(Vec2(pos_.x, pos_.y));
	this->setScale(1.5f);
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::Cultist;

	// �A�j���[�V�����̓o�^
	AnimRegistrator();
	// ActModule�̓o�^
	//ActModuleRegistrator();

	currentAnimation_ = "idle";
	previousAnimation_ = currentAnimation_;

	this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));

	direction_ = Direction::Left;

	//lpCol.Load(collider_, "idle", "assassin");
	// �U����`�̃T�C�Y�ݒ�
	attackRect_.size_ = Size(30.0f, 30.0f);
	//for (auto col : collider_["idle"])
	//{
	//	for (int colNum = 0; colNum < col.size(); colNum++)
	//	{
	//		// colliderBox�����g�̎q�ɂ���
	//		auto draw = col[colNum]->create();
	//		// �|�W�V��������������...
	//		draw->setAnchorPoint(Vec2(0.0f, 0.0f));
	//		draw->setContentSize(Size{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y });
	//		draw->drawRect(Vec2(0, 0), Vec2{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y }, col[colNum]->GetColor());
	//		draw->setTag(col[colNum]->GetData().frame_);
	//		this->addChild(draw, 0, "idle");
	//	}
	//}

	for (auto anim : lpAnimMng.GetAnimations(type_))
	{
		// colliderBox��Load
		lpCol.Load(collider_, anim, "cultist");
		for (auto col : collider_[anim])
		{
			for (int colNum = 0; colNum < col.size(); colNum++)
			{
				// colliderBox�����g�̎q�ɂ���
				auto draw = col[colNum]->create();
				draw->setContentSize(Size{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y });
				draw->drawRect(Vec2(0, 0), Vec2{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y }, col[colNum]->GetColor());

				draw->setTag(col[colNum]->GetData().frame_);

				this->addChild(draw, col[colNum]->GetData().type_, anim);
			}
		}
	}
}

Cultist::~Cultist()
{
}

Cultist* Cultist::CreateCultist(Player& player, BehaviorTree* aiTree, VisionRange visionRange, int hp)
{
	Cultist* pRet = new(std::nothrow) Cultist(player, aiTree, visionRange, hp);
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

void Cultist::Action(void)
{
}

void Cultist::update(float delta)
{
	// ���񂾔���
	if (getName() == "death")
	{
		// ������e�ł���GameScene����폜����
		removeFromParentAndCleanup(true);
	}
	else
	{
		if (!isAttacking_)
		{
			// �����̕ύX
			ChangeDirection();
		}
		// ���݂̃t���[���𐮐��l�Ŏ擾
		animationFrame_int_ = GetAnimationFrameInt() - 1;
		// 0�ȉ��ɂȂ��0�ɂ���
		if (animationFrame_int_ < 0)
		{
			animationFrame_int_ = 0;
		}
		// �������ɒʂ��Ă����珈�����d���Ȃ�̂�
		// ��ڲ԰���U���Ԑ��Ŏ������U���H����Ă��Ȃ�������(���̂Ƃ��낱�����Ă��邪�A��Ŏ�����hitӰ��݂��I�������ɕύX�\��)
		if (player_.IsAttacking() && !onDamaged_)
		{
			// ��ڲ԰�Ƃ̓����蔻����Ƃ��Ă���
			CheckHitPLAttack();
		}

		// ��Ұ�ނ�H�����
		if (onDamaged_ && stateTransitioner_ != &Enemy::Hit)
		{
			// hp�����
			hp_ -= 10;
			// hp��0�ɂȂ�����
			if (hp_ <= 0)
			{
				if (stateTransitioner_ != &Enemy::Death)
				{
					// ���ʏ�Ԃɂ���
					ChangeAnimation("death");
					stateTransitioner_ = &Enemy::Death;
				}
			}
			else
			{
				// 0�ł͂Ȃ�������hit��Ԃɂ���
				ChangeAnimation("hit");
				stateTransitioner_ = &Enemy::Hit;
			}
		}

		//if (currentAnimation_ == "attack")
		{
			currentCol_ = collider_[currentAnimation_][animationFrame_int_];
		}

		//previousAnimation_ = currentAnimation_;

		TRACE("pos:(%f,%f)", this->getPosition().x, this->getPosition().y);

		TRACE("attackFlag:%d\n", isAttacking_);
		if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
		{
			animationFrame_ = 0.0f;
			return;
		}


		for (auto animationCol = this->getChildren().rbegin();
			animationCol != this->getChildren().rend(); animationCol++)
		{
			if (currentAnimation_ == (*animationCol)->getName() &&
				animationFrame_int_ == (*animationCol)->getTag())
			{
				(*animationCol)->setVisible(true);
			}
			else
			{
				(*animationCol)->setVisible(false);
			}
		}
		// ��Ԃ̑J��
		(this->*stateTransitioner_)();
		// Map�I�u�W�F�N�g�ɓ������Ă��邩�̊m�F
		//CheckMapObjHit(delta);
		// �d�͂�������
		gravity_->ApplyGravityToTarget(delta);
		// �A�j���[�V�����̍X�V
		UpdateAnimation(delta);
		// �A�j���[�V�����I�����ɍU���t���O��false
		if (isAnimEnd_)
		{
			isAttacking_ = false;
			hittingToPlayer_ = false;
		}
	}
}

void Cultist::AnimRegistrator(void)
{
	// �A�j���[�V�������L���b�V���ɓo�^
	// idle
	// ��������ύX�����ق����悢
	// �p�X������������ł��ď����Ӗ��Ȃ� ex.)image/EnemyAnimationAsset/assassin�ł���
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "idle", 6, 0.03f, ActorType::Cultist, true);
	// run
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "walk", 8, 0.08f, ActorType::Cultist, true);

	// attack
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "attack", 10, 0.08f, ActorType::Cultist, false);

	// death
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "death", 12, 0.08f, ActorType::Cultist, false);

	// hit 
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/cultist/cultist", "hit", 3, 0.09f, ActorType::Cultist, false);
	// �����A�j���[�V�����̃Z�b�g
	lpAnimMng.InitAnimation(*this, ActorType::Cultist, "idle");
}

void Cultist::AddAttackObj(void)
{
	//auto fireball = Fireball::CreateFireball(getPosition(), type_);

	//this->addChild(fireball);

	//fireball->scheduleUpdate();
}

void Cultist::NormalAttack(void)
{
	isAttacking_ = true;


}

void Cultist::UseSkill(void)
{
}

void Cultist::Patrol(void)
{
}

void Cultist::Chase(void)
{
}

void Cultist::Run(void)
{
}

void Cultist::Jump(void)
{
}

void Cultist::Fall(void)
{
}