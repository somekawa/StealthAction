#include "BigCultist.h"
#include "Gravity.h"
#include "_Debug/_DebugConOut.h"
#include "ActionRect.h"
#include "Loader/CollisionLoader.h"
#include "anim/AnimMng.h"
#include "obj/Player.h"

#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"

USING_NS_CC;

BigCultist::BigCultist(cocos2d::Vec2 pos, Player& player, BehaviorTree* aiTree, VisionRange visionRange, int hp, cocos2d::Layer& myLayer) :
	Enemy(pos, player, aiTree, visionRange, hp, myLayer)
{
	this->setScale(3.0f);
	myName_ = "bigCultist";
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::BigCultist;

	// �A�j���[�V�����̓o�^
	AnimRegistrator();
	// ActModule�̓o�^
	//ActModuleRegistrator();

	currentAnimation_ = "bigCultist_idle";
	previousAnimation_ = currentAnimation_;

	//this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_,currentAnimation_)));

	direction_ = Direction::Left;

	//lpCol.Load(collider_, "idle", "assassin");
	// �U����`�̃T�C�Y�ݒ�
	attackRect_.size_ = Size(30.0f, 30.0f);

	for (auto anim : lpAnimMng.GetAnimations(type_))
	{
		// colliderBox��Load
		lpCol.ReadData(collider_, anim);
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

BigCultist::~BigCultist()
{
}

BigCultist* BigCultist::CreateBigCultist(cocos2d::Vec2 pos, Player& player, BehaviorTree* aiTree, VisionRange visionRange, int hp, cocos2d::Layer& myLayer)
{
	BigCultist* pRet = new(std::nothrow) BigCultist(pos, player, aiTree, visionRange, hp, myLayer);
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

void BigCultist::Action(void)
{
}

void BigCultist::update(float delta)
{
	if (getName() == "assassin_death" || getName() == "changeFloor_death")
	{
		// ������e�ł���GameScene����폜����
		this->removeFromParentAndCleanup(true);
	}
	else
	{
		previousAnimation_ = currentAnimation_;
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

void BigCultist::AnimRegistrator(void)
{
}

void BigCultist::AddAttackObj(const float& angle)
{
}

void BigCultist::NormalAttack(void)
{
	isAttacking_ = true;
	if (animationFrame_int_ < 21)
	{
		currentCol_ = collider_[currentAnimation_][animationFrame_int_];
	}
	if (OnAttacked())
	{
		player_.OnDamaged();
	}
	TRACE("direction:%d", direction_);
}

void BigCultist::UseSkill(void)
{
}

void BigCultist::Patrol(void)
{
	if (isMoveComplete_)
	{
		// ��苗�������Ă�����
		oldPos_ = getPosition();
		isMoveComplete_ = false;
	}
	else
	{
		// �����������������苗���ɂȂ�܂�
		if (getPosition().x >= oldPos_.x + 30.0f)
		{
			isMoveComplete_ = true;
		}
	}
	auto previousPos = getPosition().x;
	// ���񏈗�
	switch (direction_)
	{
	case Direction::Right:
		speed_.x = 1;
		flipFlag_ = FlipX::create(true);
		break;
	case Direction::Left:
		speed_.x = -1;
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
	currentAnimation_ = "bigCultist_run";
	auto move = MoveTo::create(0.0f, Vec2(previousPos + speed_.x, getPosition().y));
	//pos_.x += speed_.x;
	//this->setPosition(Vec2(pos_.x, pos_.y));
	this->runAction(move);
	this->runAction(flipFlag_);
	setPosition(Vec2(previousPos + speed_.x, getPosition().y));
}

void BigCultist::Chase(void)
{
	auto previousPos = getPosition().x;

	// �v���C���[��ǂ������鏈��
	switch (direction_)
	{
	case Direction::Right:
		speed_.x = 2;
		flipFlag_ = FlipX::create(true);
		break;
	case Direction::Left:
		speed_.x = -2;
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
	currentAnimation_ = "bigCultist_run";
	auto move = MoveTo::create(0.0f, Vec2(previousPos + speed_.x, getPosition().y));
	//pos_.x += speed_.x;
	//this->setPosition(Vec2(pos_.x, pos_.y));
	this->runAction(move);
	this->runAction(flipFlag_);
	setPosition(Vec2(previousPos + speed_.x, getPosition().y));
}

void BigCultist::Run(void)
{
}

void BigCultist::Jump(void)
{
}

void BigCultist::Fall(void)
{
}