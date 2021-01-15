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

Assassin::Assassin(Vec2 pos, Player& player,
	BehaviorTree* aiTree,VisionRange visionRange,int hp,Layer& myLayer):
	Enemy(pos,player,aiTree,visionRange,hp,myLayer)
{
	//pos_ = { 500,500 };
	//this->setPosition(Vec2(pos_.x, pos_.y));
	this->setScale(1.5f);
	myName_ = "assassin";
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::Assassin;

	// �A�j���[�V�����̓o�^
	AnimRegistrator();
	// ActModule�̓o�^
	actModuleRegistration();

	currentAnimation_ = "assassin_idle";
	previousAnimation_ = currentAnimation_;
	direction_ = Direction::Left;

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

				this->addChild(draw,col[colNum]->GetData().type_, anim);
			}
		}
	}
	// �����A�j���[�V�����̃Z�b�g
	lpAnimMng.InitAnimation(*this, ActorType::Assassin, "assassin_idle");
}

Assassin::~Assassin()
{

}

Assassin* Assassin::CreateAssassin(Vec2 pos, Player& player,
	BehaviorTree* aiTree,VisionRange visionRange,int hp,Layer& myLayer)
{
	Assassin* pRet = new(std::nothrow) Assassin(pos,player,aiTree,visionRange,hp,myLayer);
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
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		animationFrame_ = 0.0f;
		animationFrame_int_ = 0.0f;
		return;
	}
	// �۱�ύX�̍ۂɎ��g������
	//DeleteSelfOnFloor();
	// ���񂾔���
	if (getName() == "assassin_death" || getName() == "changeFloor_death")
	{
		// ������e�ł���GameScene����폜����
		this->removeFromParentAndCleanup(true);
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
		
		// ��ڲ԰���U����Ԃ��Ɠ����蔻�菈��������
		if (player_.IsAttacking())
		{
			// ��ڲ԰�Ƃ̓����蔻����Ƃ��Ă���
			CheckHitPLAttack();
		}
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
					ChangeAnimation("assassin_hit");
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
				ChangeAnimation("assassin_death");
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
			currentAnimation_ = "assassin_idle";
		}

		if (currentAnimation_ != previousAnimation_)
		{
			ChangeAnimation(currentAnimation_);
			//onDamaged_ = false;
		}
		previousAnimation_ = currentAnimation_;

	}

}

void Assassin::AnimRegistrator(void)
{



}

void Assassin::AddAttackObj(const float& angle)
{
}

void Assassin::actModuleRegistration(void)
{
	Vec2 size = { 106.0f,76.0f };

	// �E�������]
	{
		ActModule flipAct;
		flipAct.state = nullptr;
		flipAct.flipFlg = true;
		flipAct.actName = "assassin_idle";
		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("assassin_attack");
		actCtl_.RunInitializeActCtl(type_, "�E����", flipAct);
	}

	// ���������]
	{
		ActModule flipAct;
		flipAct.state = nullptr;
		flipAct.flipFlg = false;
		flipAct.actName = "assassin_idle";

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("assassin_attack");
		actCtl_.RunInitializeActCtl(type_, "������", flipAct);
	}

	// �E�ړ�
	{
		ActModule act;
		act.state = nullptr;
		act.vel = Vec2{ 2,0 };
		act.actName = "assassin_run";
		act.checkPoint1 = Vec2{ size.x / 2, size.y / 2 };	// �E��
		act.checkPoint2 = Vec2{ size.x / 2,  15 };			// �E��
		//act.blackList.emplace_back(ACTION::FALLING);	// �������ɉE�ړ����Ăق����Ȃ��Ƃ��̒ǉ��̎d��

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("assassin_attack");
		actCtl_.RunInitializeActCtl(type_,"�E�ړ�", act);
	}

	// ���ړ�
	{
		ActModule act;
		act.state = nullptr;
		act.vel = Vec2{ 2,0 };
		act.actName = "assassin_run";
		act.checkPoint1 = Vec2{ -size.x / 2, size.y / 2 };	// ����
		act.checkPoint2 = Vec2{ -size.x / 2,  15 };			// ����

		//act.blackList.emplace_back(ACTION::FALLING);

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("assassin_attack");
		actCtl_.RunInitializeActCtl(type_,"���ړ�", act);
	}


	// ����
	{
		// checkkeylist�ɗ����Ă���Ԃ̐ݒ���������Ǔ��Ɍ��ʂȂ�
		ActModule act;
		act.actName = "assassin_fall";
		act.state = nullptr;
		//act.checkPoint1 = Vec2{ 0,-10 };			// ����
		//act.checkPoint2 = Vec2{ 0,-10 };			// �E��
		act.checkPoint1 = Vec2{ -60, -60 };				// ����
		act.checkPoint2 = Vec2{ -60, -60 };				// �E��

		act.checkPoint3 = Vec2{ size.x / 2, size.y / 2 };  // �E��
		act.checkPoint4 = Vec2{ -size.x / 2, size.y / 2 }; // ����

		act.gravity = Vec2{ 0.0f,-5.0f };
		act.blackList.emplace_back("assassin_attack");	// �W�����v���ɗ������Ăق����Ȃ�
		//act.blackList.emplace_back(ACTION::JUMP);	// �W�����v���ɗ������Ăق����Ȃ�

		actCtl_.RunInitializeActCtl(type_, "����", act);
	}
	// �U��
	{
		ActModule act;
		act.state = nullptr;
		act.actName = "assassin_attack";
		actCtl_.RunInitializeActCtl(type_, "�U��", act);
	}
	// �X�V�֐��̓o�^
	actCtl_.InitUpdater(type_);
}

void Assassin::Jump(void)
{
}

void Assassin::Fall(void)
{
}

void Assassin::NormalAttack(void)
{
	//isAttacking_ = true;
	if (animationFrame_int_ < 13)
	{
		currentCol_ = collider_[currentAnimation_][animationFrame_int_];
	}
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
	currentAnimation_ = "assassin_run";
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
	currentAnimation_ = "assassin_run";
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
	if (GetDistance() < 50.0f)
	{
		// �A�j���[�V������ύX����
		ChangeAnimation("assassin_attack");
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
		ChangeAnimation("assassin_idle");
		// �ύX�����A�j���[�V�����𑖂点��
		this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
		// �s����idle�ɂ���
		updater_ = &Assassin::Idle;
	}
}
