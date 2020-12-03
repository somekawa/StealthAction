#include "Assassin.h"
#include "Gravity.h"
#include "_Debug/_DebugConOut.h"
#include "ActionRect.h"
#include "Loader/CollisionLoader.h"
#include "anim/AnimMng.h"

#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"

USING_NS_CC;

Assassin::Assassin(cocos2d::Vector<cocos2d::Node*>& player, BehaviorTree* aiTree):
	Enemy(player,aiTree)
{
	pos_ = { 500,500 };
	this->setPosition(Vec2(pos_.x, pos_.y));
	this->setScale(2.0f);
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

	lpCol.Load(collider_, "idle", "assassin");

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
				this->addChild(draw, 0, anim);
			}
		}
	}

	//updater_ = &Assassin::Idle;
}

Assassin::~Assassin()
{

}

Assassin* Assassin::CreateAssassin(cocos2d::Vector<cocos2d::Node*>& player, BehaviorTree* aiTree)
{
	Assassin* pRet = new(std::nothrow) Assassin(player,aiTree);
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
	if (!isAttacking_)
	{
		// �����̕ύX
		ChangeDirection();
	}
	//previousAnimation_ = currentAnimation_;
	// AI�N��
	AIRun();
	TRACE("attackFlag:%d\n", isAttacking_);
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		animationFrame_ = 0.0f;
		return;
	}
	// �d�͂�������
	gravity_->ApplyGravityToTarget(delta);
	Action();

	// ���݂̃t���[���𐮐��l�Ŏ擾
	animationFrame_int_ = GetAnimationFrameInt();
	for (auto animationCol = this->getChildren().rbegin();
		animationCol != this->getChildren().rend(); animationCol++)
	{
		TRACE("animationInt:%d\n", animationFrame_int_);
		auto i = (*animationCol)->getTag()-2;
		TRACE("i:%d,animInt:%d\n", i,animationFrame_int_);
		if (currentAnimation_ == (*animationCol)->getName() &&
			animationFrame_int_==i)
		{
			(*animationCol)->setVisible(true);
		}
		else
		{
			(*animationCol)->setVisible(false);
		}
	}
	// �R���C�_�[�̕`��
	//for (auto collider : this->getChildren())
	//{
	//	if (currentAnimation_ == collider->getName()/* &&
	//		animationFrame_int_ == collider->getTag()*/)
	//	{
	//		collider->setVisible(true);
	//	}
	//	else
	//	{
	//		collider->setVisible(false);
	//	}
	//}
	// �A�j���[�V�����̍X�V
	UpdateAnimation(delta);
	// �A�j���[�V�����I�����ɍU���t���O��false
	if (isAnimEnd_)
	{
		isAttacking_ = false;
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
	lpAnimMng.addAnimationCache("image/EnemyAnimationAsset/assassin/assassin", "death", 16, 1.0f, ActorType::Assassin, false);
	// �����A�j���[�V�����̃Z�b�g
	lpAnimMng.InitAnimation(*this, ActorType::Assassin, "idle");
}

void Assassin::Jump(void)
{
}

void Assassin::Fall(void)
{
}

void Assassin::Hit(void)
{
}

void Assassin::Death(void)
{
}

void Assassin::NormalAttack(void)
{
	isAttacking_ = true;
	//TRACE("attackFlag:%d\n", isAttacking_);
	//currentAnimation_ = "attack";
	if (isAnimEnd_)
	{
		isAttacking_ = false;
		//TRACE("attackFlag:%d\n", isAttacking_);

		ChangeAnimation("idle");
	}
}

void Assassin::UseSkill(void)
{
}

void Assassin::Idle(void)
{

}

void Assassin::Patrol(void)
{
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
	pos_.x += speed_.x;
	this->setPosition(Vec2(pos_.x, pos_.y));
	this->runAction(flipFlag_);
}

void Assassin::Chase(void)
{
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
	pos_.x += speed_.x;
	this->setPosition(Vec2(pos_.x, pos_.y));
	this->runAction(flipFlag_);
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
