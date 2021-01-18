#include "TwistedCultist.h"
#include "_Debug/_DebugConOut.h"
#include "Loader/CollisionLoader.h"
#include "ActionRect.h"
#include "anim/AnimMng.h"
#include "obj/Player.h"
#include "Gravity.h"

USING_NS_CC;

TwistedCultist::TwistedCultist(Vec2 pos,Player& player,
	BehaviorTree* aiTree,VisionRange visionRange,int hp,Layer& myLayer):
	Enemy(pos,player,aiTree,visionRange,hp,myLayer)
{
	//pos_ = { 500,500 };
	//this->setPosition(Vec2(pos_.x, pos_.y));
	this->setScale(2.0f);
	myName_ = "twistedCultist";
	//this->setAnchorPoint(Vec2(0.5f, 0.0f));
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::TwistedCultist;

	// �A�j���[�V�����̓o�^
	AnimRegistrator();
	actModuleRegistration();

	currentAnimation_ = "twistedCultist_idle";
	//this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
	direction_ = Direction::Left;
	for (auto anim : lpAnimMng.GetAnimations(type_))
	{
		// colliderBox��Load
		lpCol.ReadData(collider_, anim);
		for (auto col : collider_[anim])
		{
			for (int colNum = 0; colNum < col.size(); colNum++)
			{
				// colliderBox�����g�̎q�ɂ���
				auto draw = DrawNode::create();
				draw->setContentSize(Size{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y });
				draw->drawRect(Vec2(0, 0), Vec2{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y }, col[colNum]->GetColor());

				draw->setTag(col[colNum]->GetData().frame_);
				this->addChild(draw, col[colNum]->GetData().type_, anim);
			}
		}
	}
	// �����A�j���[�V�����̃Z�b�g
	lpAnimMng.InitAnimation(*this, ActorType::TwistedCultist, "twistedCultist_idle");
	updater_ = &TwistedCultist::Idle;
}

TwistedCultist::~TwistedCultist()
{
}

TwistedCultist* TwistedCultist::CreateTwistedCultist(Vec2 pos,Player& player, 
	BehaviorTree* aiTree, VisionRange visionRange,int hp,Layer& myLayer)
{
	TwistedCultist* pRet = new(std::nothrow) TwistedCultist(pos,player,aiTree,visionRange,hp,myLayer);
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
	//ChangeDirection();
	(this->*updater_)();
}

void TwistedCultist::update(float delta)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		animationFrame_ = 0.0f;
		animationFrame_int_ = 0.0f;
		return;
	}
	// ���񂾔���
	if (getName() == "twistedCultist_death" || getName() == "changeFloor_death")
	{
		// ������e�ł���GameScene����폜����
		this->removeFromParentAndCleanup(true);
	}
	else
	{
		setAnchorPoint(Vec2(0.0f, 0.0f));

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
		currentCol_ = collider_[currentAnimation_][animationFrame_int_];

		//// ��ڲ԰���U����Ԃ��Ɠ����蔻�菈��������
		//if (player_.IsAttacking())
		//{
		//	// ��ڲ԰�Ƃ̓����蔻����Ƃ��Ă���
		//	CheckHitPLAttack();
		//}
		SetCollider();
		// ��Ұ�ނ�������Ă��Ȃ����Ǝ���Ӱ��݂łȂ��ꍇ
		//if (!onDamaged_ && stateTransitioner_ != &Enemy::Death)
		//{
		//	// actCtl�̱����ްĂ���
		//	actCtl_.update(type_, delta, *this);
		//}

		if (stateTransitioner_ != &Enemy::Death)
		{
			// ��Ұ�ނ�H�����
			if (onDamaged_)
			{
				if (stateTransitioner_ != &Enemy::Hit)
				{

					//onDamaged_ = false;
					ChangeAnimation("twistedCultist_hit");
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
				ChangeAnimation("twistedCultist_death");
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
			currentAnimation_ = "twistedCultist_idle";
		}

		if (currentAnimation_ != previousAnimation_)
		{
			ChangeAnimation(currentAnimation_);
			//onDamaged_ = false;
		}
		previousAnimation_ = currentAnimation_;

	}
	// �۱�ύX�̍ۂɎ��g������
	//DeleteSelfOnFloor();
}

void TwistedCultist::AnimRegistrator(void)
{


}

void TwistedCultist::AddAttackObj(const float& angle)
{
}

void TwistedCultist::actModuleRegistration(void)
{
	Vec2 size = { 45.0f,42.0f };

	// �E�ړ�
	{
		ActModule act;
		act.state = nullptr;
		act.vel = Vec2{ 0.5f,0 };
		act.actName = "twistedCultist_walk";
		act.checkPoint1 = Vec2{ size.x / 2, size.y / 2 };	// �E��
		act.checkPoint2 = Vec2{ -40,0 };			// �E��
		//act.blackList.emplace_back(ACTION::FALLING);	// �������ɉE�ړ����Ăق����Ȃ��Ƃ��̒ǉ��̎d��

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("twistedCultist_attack");
		actCtl_.RunInitializeActCtl(type_, "�E�ړ�", act);
	}

	// ���ړ�
	{
		ActModule act;
		act.state = nullptr;
		act.vel = Vec2{ 0.5f,0 };
		act.actName = "twistedCultist_walk";
		act.checkPoint1 = Vec2{ -size.x / 2, size.y / 2 };	// ����
		act.checkPoint2 = Vec2{ -40,-40 };			// ����

		//act.blackList.emplace_back(ACTION::FALLING);

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("twistedCultist_attack");
		actCtl_.RunInitializeActCtl(type_, "���ړ�", act);
	}

	// �E�������]
	{
		ActModule flipAct;
		flipAct.state = nullptr;
		flipAct.flipFlg = true;
		flipAct.actName = "twistedCultist_idle";
		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("twistedCultist_attack");
		actCtl_.RunInitializeActCtl(type_, "�E����", flipAct);
	}

	// ���������]
	{
		ActModule flipAct;
		flipAct.state = nullptr;
		flipAct.flipFlg = false;
		flipAct.actName = "twistedCultist_idle";

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("twistedCultist_attack");
		actCtl_.RunInitializeActCtl(type_, "������", flipAct);
	}
	// ����
	{
		// checkkeylist�ɗ����Ă���Ԃ̐ݒ���������Ǔ��Ɍ��ʂȂ�
		ActModule act;
		act.actName = "twistedCultist_fall";
		act.state = nullptr;
		//act.checkPoint1 = Vec2{ 0,-10 };			// ����
		//act.checkPoint2 = Vec2{ 0,-10 };			// �E��
		act.checkPoint1 = Vec2{ -40,-40 };				// ����
		act.checkPoint2 = Vec2{ -40,-40 };				// �E��

		act.checkPoint3 = Vec2{ size.x / 2, size.y / 2 };  // �E��
		act.checkPoint4 = Vec2{ -size.x / 2, size.y / 2 }; // ����

		act.gravity = Vec2{ 0.0f,-5.0f };
		act.blackList.emplace_back("twistedCultist_attack");	// �W�����v���ɗ������Ăق����Ȃ�
		//act.blackList.emplace_back(ACTION::JUMP);	// �W�����v���ɗ������Ăق����Ȃ�

		actCtl_.RunInitializeActCtl(type_, "����", act);
	}
	// �U��
	{
		ActModule act;
		act.state = nullptr;
		act.actName = "twistedCultist_attack";
		actCtl_.RunInitializeActCtl(type_, "�U��", act);
	}
	// �X�V�֐��̓o�^
	actCtl_.InitUpdater(type_);
}

void TwistedCultist::NormalAttack(void)
{
	//isAttacking_ = true;
	if (animationFrame_int_ < 7)
	{
		currentCol_ = collider_[currentAnimation_][animationFrame_int_];
	}
	if (OnAttacked())
	{
		player_.OnDamaged();
	}
	TRACE("direction:%d", direction_);
}

void TwistedCultist::UseSkill(void)
{
}

void TwistedCultist::Patrol(void)
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
	currentAnimation_ = "twistedCultist_walk";
	auto move = MoveTo::create(0.0f, Vec2(previousPos + speed_.x, getPosition().y));
	//pos_.x += speed_.x;
	//this->setPosition(Vec2(pos_.x, pos_.y));
	this->runAction(move);
	this->runAction(flipFlag_);
	setPosition(Vec2(previousPos + speed_.x, getPosition().y));
}

void TwistedCultist::Chase(void)
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
	currentAnimation_ = "twistedCultist_walk";
	auto move = MoveTo::create(0.0f, Vec2(previousPos + speed_.x, getPosition().y));
	//pos_.x += speed_.x;
	//this->setPosition(Vec2(pos_.x, pos_.y));
	this->runAction(move);
	this->runAction(flipFlag_);
	setPosition(Vec2(previousPos + speed_.x, getPosition().y));
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
