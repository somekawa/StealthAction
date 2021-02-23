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
	actModuleRegistration();
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
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		animationFrame_ = 0.0f;
		return;
	}
	if (getName() == "bigCultist_death" || getName() == "changeFloor_death")
	{
		// ������e�ł���GameScene����폜����
		this->removeFromParentAndCleanup(true);
	}
	else
	{
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

		//// ��ڲ԰���U����Ԃ��Ɠ����蔻�菈��������
		//if (player_.IsAttacking())
		//{
		//	// ��ڲ԰�Ƃ̓����蔻����Ƃ��Ă���
		//	CheckHitPLAttack();
		//}
		// ��Ұ�ނ�������Ă��Ȃ����Ǝ���Ӱ��݂łȂ��ꍇ
		if (!onDamaged_ && stateTransitioner_ != &Enemy::Death)
		{
			// actCtl�̱����ްĂ���
			actCtl_.update(type_, delta, *this);
		}

		if (stateTransitioner_ != &Enemy::Death)
		{
			// ��Ұ�ނ�H�����
			if (onDamaged_)
			{
				if (stateTransitioner_ != &Enemy::Hit)
				{

					//onDamaged_ = false;
					ChangeAnimation("bigCultist_hit");
					//currentAnimation_ = "assassin_hit";
					// 0�ł͂Ȃ�������hit��Ԃɂ���
					//ChangeAnimation("assassin_hit");
					stateTransitioner_ = &Enemy::Hit;
				}
			}
		}
		// hp��0�ɂȂ�����
		if (hp_ <= 0)
		{
			if (stateTransitioner_ != &Enemy::Death)
			{
				//currentAnimation_ = "assassin_death";
				ChangeAnimation("bigCultist_death");
				stateTransitioner_ = &Enemy::Death;
			}
		}
		TRACE("pos:(%f,%f)", this->getPosition().x, this->getPosition().y);

		TRACE("attackFlag:%d\n", isAttacking_);

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

		// �A�j���[�V�����̍X�V
		UpdateAnimation(delta);
		// ��Ԃ̑J��
		(this->*stateTransitioner_)();

		// �A�j���[�V�����I�����ɍU���t���O��false
		if (isAnimEnd_)
		{
			isAttacking_ = false;
			hittingToPlayer_ = false;
			stateTransitioner_ = &Enemy::Idle;
			//onDamaged_ = false;
			currentAnimation_ = "bigCultist_idle";
		}

		if (currentAnimation_ != previousAnimation_)
		{
			ChangeAnimation(currentAnimation_);
			//onDamaged_ = false;
		}
		previousAnimation_ = currentAnimation_;
	}

}

void BigCultist::AnimRegistrator(void)
{
}

void BigCultist::AddAttackObj(const float& angle)
{
}

void BigCultist::actModuleRegistration(void)
{
	Vec2 size = { 108.0f,59.0f };

	// �E�ړ�
	{
		ActModule act;
		act.state = nullptr;
		act.vel = Vec2{ 0.5f,0 };
		act.actName = "bigCultist_Run";
		act.checkPoint1 = Vec2{ size.x / 2, size.y / 2 };	// �E��
		act.checkPoint2 = Vec2{ size.x / 2,  15 };			// �E��
		//act.blackList.emplace_back(ACTION::FALLING);	// �������ɉE�ړ����Ăق����Ȃ��Ƃ��̒ǉ��̎d��

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("bigCultist_attack");
		actCtl_.RunInitializeActCtl(type_,"�E�ړ�", act);
	}

	// ���ړ�
	{
		ActModule act;
		act.state = nullptr;
		act.vel = Vec2{ 0.5f,0 };
		act.actName = "bigCultist_Run";
		act.checkPoint1 = Vec2{ -size.x / 2, size.y / 2 };	// ����
		act.checkPoint2 = Vec2{ -size.x / 2,  15 };			// ����

		//act.blackList.emplace_back(ACTION::FALLING);

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("bigCultist_attack");
		actCtl_.RunInitializeActCtl(type_,"���ړ�", act);
	}

	// �E�������]
	{
		ActModule flipAct;
		flipAct.state = nullptr;
		flipAct.flipFlg = true;
		flipAct.actName = "bigCultist_idle";
		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("bigCultist_attack");
		actCtl_.RunInitializeActCtl(type_,"�E����", flipAct);
	}

	// ���������]
	{
		ActModule flipAct;
		flipAct.state = nullptr;
		flipAct.flipFlg = false;
		flipAct.actName = "bigCultist_idle";

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("bigCultist_attack");
		actCtl_.RunInitializeActCtl(type_,"������", flipAct);
	}
	// ����
	{
		// checkkeylist�ɗ����Ă���Ԃ̐ݒ���������Ǔ��Ɍ��ʂȂ�
		ActModule act;
		act.actName = "bigCultist_fall";
		act.state = nullptr;
		//act.checkPoint1 = Vec2{ 0,-10 };			// ����
		//act.checkPoint2 = Vec2{ 0,-10 };			// �E��
		act.checkPoint1 = Vec2{ 0,0 };				// ����
		act.checkPoint2 = Vec2{ 0,0 };				// �E��

		act.checkPoint3 = Vec2{ size.x / 2, size.y / 2 };  // �E��
		act.checkPoint4 = Vec2{ -size.x / 2, size.y / 2 }; // ����

		act.gravity = Vec2{ 0.0f,-5.0f };
		act.blackList.emplace_back("bigCultist_attack");	// �W�����v���ɗ������Ăق����Ȃ�
		//act.blackList.emplace_back(ACTION::JUMP);	// �W�����v���ɗ������Ăق����Ȃ�

		actCtl_.RunInitializeActCtl(type_, "����", act);
	}

	// �U��
	{
		ActModule act;
		act.state = nullptr;
		act.actName = "bigCultist_attack";
		actCtl_.RunInitializeActCtl(type_, "�U��", act);
	}
	// �X�V�֐��̓o�^
	actCtl_.InitUpdater(type_);
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
