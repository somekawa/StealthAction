#include "Assassin.h"
#include "Gravity.h"
#include "_Debug/_DebugConOut.h"
#include "ActionRect.h"
#include "Loader/CollisionLoader.h"
#include "anim/AnimMng.h"
#include "obj/Player.h"

#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"

USING_NS_CC;

Assassin::Assassin(Player& player,
	BehaviorTree* aiTree,VisionRange visionRange,int hp,Layer& myLayer):
	Enemy(player,aiTree,visionRange,hp,myLayer)
{
	pos_ = { 500,500 };
	this->setPosition(Vec2(pos_.x, pos_.y));
	this->setScale(1.5f);
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::Assassin;

	// �A�j���[�V�����̓o�^
	AnimRegistrator();
	// ActModule�̓o�^
	//ActModuleRegistrator();

	currentAnimation_ = "idle";
	previousAnimation_ = currentAnimation_;

	this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_,currentAnimation_)));

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
		lpCol.Load(collider_, anim, "assassin");
		for (auto col : collider_[anim])
		{
			for (int colNum = 0; colNum < col.size(); colNum++)
			{
				// colliderBox�����g�̎q�ɂ���
				auto draw = col[colNum]->create();
				draw->setContentSize(Size{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y });
				draw->drawRect(Vec2(0, 0), Vec2{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y }, col[colNum]->GetColor());

				draw->setTag(col[colNum]->GetData().frame_);

				this->addChild(draw,col[colNum]->GetData().type_, anim);
			}
		}
	}
}

Assassin::~Assassin()
{

}

Assassin* Assassin::CreateAssassin(Player& player,
	BehaviorTree* aiTree,VisionRange visionRange,int hp,Layer& myLayer)
{
	Assassin* pRet = new(std::nothrow) Assassin(player,aiTree,visionRange,hp,myLayer);
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
	//(this->*updater_)();
}

void Assassin::update(float delta)
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
		animationFrame_int_ = GetAnimationFrameInt()-1;
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

void Assassin::AnimRegistrator(void)
{
	// �A�j���[�V�������L���b�V���ɓo�^
	// idle
	// ��������ύX�����ق����悢
	// �p�X������������ł��ď����Ӗ��Ȃ� ex.)image/EnemyAnimationAsset/assassin�ł���
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "idle", 8, 0.03f, ActorType::Assassin, true);
	// run
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "run", 8, 0.08f, ActorType::Assassin, true);

	// attack
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "attack", 13, 0.08f, ActorType::Assassin, false);

	// death
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "death", 16, 0.08f, ActorType::Assassin, false);

	// hit 
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "hit", 3, 0.09f, ActorType::Assassin, false);
	// �����A�j���[�V�����̃Z�b�g
	lpAnimMng.InitAnimation(*this, ActorType::Assassin, "idle");
}

void Assassin::AddAttackObj(const float& angle)
{
}

void Assassin::Jump(void)
{
}

void Assassin::Fall(void)
{
}

void Assassin::NormalAttack(void)
{
	isAttacking_ = true;

	if (OnAttacked())
	{
		player_.OnDamaged();
	}
	TRACE("direction:%d", direction_);
}

void Assassin::UseSkill(void)
{
}

void Assassin::Patrol(void)
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
	currentAnimation_ = "run";
	auto move = MoveTo::create(0.0f, Vec2(previousPos + speed_.x,getPosition().y));
	//pos_.x += speed_.x;
	//this->setPosition(Vec2(pos_.x, pos_.y));
	this->runAction(move);
	this->runAction(flipFlag_);
	setPosition(Vec2(previousPos + speed_.x, getPosition().y));
}

void Assassin::Chase(void)
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
	currentAnimation_ = "run";
	auto move = MoveTo::create(0.0f, Vec2(previousPos + speed_.x, getPosition().y));
	//pos_.x += speed_.x;
	//this->setPosition(Vec2(pos_.x, pos_.y));
	this->runAction(move);
	this->runAction(flipFlag_);
	setPosition(Vec2(previousPos + speed_.x, getPosition().y));
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
	if (DistanceCalcurator() < 50.0f)
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
