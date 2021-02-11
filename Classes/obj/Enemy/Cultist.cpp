#include "Cultist.h"
#include "Gravity.h"
#include "_Debug/_DebugConOut.h"
#include "ActionRect.h"
#include "Loader/CollisionLoader.h"
#include "anim/AnimMng.h"
#include "obj/Player.h"

#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"

#include "obj/Objects/Fireball.h"

USING_NS_CC;

Cultist::Cultist(Vec2 pos, Player& player, BehaviorTree* aiTree, VisionRange visionRange, int hp,Layer& myLayer):
    Enemy(pos,player, aiTree, visionRange, hp,myLayer)
{
	attackCnt_ = 0;
	myName_ = "cultist";

	//pos_ = { 500,500 };
	//this->setPosition(Vec2(pos_.x, pos_.y));
	this->setScale(2.0f);
	flipFlag_ = FlipX::create(true);
	type_ = ActorType::Cultist;

	// �A�j���[�V�����̓o�^
	AnimRegistrator();
	actModuleRegistration();
	// ActModule�̓o�^
	//ActModuleRegistrator();

	currentAnimation_ = "cultist_idle";
	previousAnimation_ = currentAnimation_;

	this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));

	direction_ = Direction::Left;

	// �U����`�̃T�C�Y�ݒ�
	attackRect_.size_ = Size(30.0f, 30.0f);

	for (auto anim : lpAnimMng.GetAnimations(type_))
	{
		// colliderBox��Load
		lpCol.ReadData(collider_, anim);
		//for (auto col : collider_[anim])
		//{
		//	for (int colNum = 0; colNum < col.size(); colNum++)
		//	{
		//		// colliderBox�����g�̎q�ɂ���
		//		auto draw = col[colNum]->create();
		//		draw->setContentSize(Size{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y });
		//		draw->drawRect(Vec2(0, 0), Vec2{ (float)col[colNum]->GetData().size_.x,(float)col[colNum]->GetData().size_.y }, col[colNum]->GetColor());

		//		draw->setTag(col[colNum]->GetData().frame_);

		//		this->addChild(draw, col[colNum]->GetData().type_, anim);
		//	}
		//}
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
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		animationFrame_ = 0.0f;
		return;
	}
	// ���񂾔���
	if (getName() == "cultist_death" || getName() == "changeFloor_death")
	{
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
		if (!onDamaged_ && stateTransitioner_ != &Enemy::Death)
		{
			// actCtl�̱����ްĂ���
			actCtl_.update(type_, delta, *this);
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
				//currentAnimation_ = "assassin_death";
				ChangeAnimation("cultist_death");
				stateTransitioner_ = &Enemy::Death;
			}
		}


		TRACE("pos:(%f,%f)", this->getPosition().x, this->getPosition().y);

		TRACE("attackFlag:%d\n", isAttacking_);

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
			currentAnimation_ = "cultist_idle";
		}

		if (currentAnimation_ != previousAnimation_)
		{
			ChangeAnimation(currentAnimation_);
			//onDamaged_ = false;
		}
		previousAnimation_ = currentAnimation_;
		// �e��`���̾��
		SetCollider();
	}
}

void Cultist::AnimRegistrator(void)
{


}

void Cultist::AddAttackObj(const float& angle)
{
	// �U�����Ă�����fireball���΂�
	auto fireball = Fireball::CreateFireball({ getPosition().x,getPosition().y + 30.0f },direction_,angle,player_);
	if (direction_ == Direction::Left)
	{
		fireball->runAction(FlipX::create(false));
	}
	else
	{
		fireball->runAction(FlipX::create(true));
	}
	// �U��ڲ԰��fireball���Ԃ牺����
	attackLayer_->addChild(fireball,0);
	attackCnt_++;
	isAttacking_ = true;
	fireball->scheduleUpdate();
}

const float Cultist::GetPLAngle(void)
{
	auto plPos = player_.getPosition();

	auto angle = atan2(getPosition().y - plPos.y,getPosition().x - plPos.x);

	return angle;
}

void Cultist::actModuleRegistration(void)
{
	Vec2 size = { 45.0f,42.0f };

	// �E�ړ�
	{
		ActModule act;
		act.state = nullptr;
		act.vel = Vec2{ 0.5f,0 };
		act.actName = "cultist_walk";
		act.checkPoint1 = Vec2{ size.x / 2, size.y / 2 };	// �E��
		act.checkPoint2 = Vec2{ size.x / 2,  15 };			// �E��
		//act.blackList.emplace_back(ACTION::FALLING);	// �������ɉE�ړ����Ăق����Ȃ��Ƃ��̒ǉ��̎d��

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("cultist_attack");
		actCtl_.RunInitializeActCtl(type_, "�E�ړ�", act);
	}

	// ���ړ�
	{
		ActModule act;
		act.state = nullptr;
		act.vel = Vec2{ 0.5f,0 };
		act.actName = "cultist_walk";
		act.checkPoint1 = Vec2{ -size.x / 2, size.y / 2 };	// ����
		act.checkPoint2 = Vec2{ -size.x / 2,  15 };			// ����

		//act.blackList.emplace_back(ACTION::FALLING);

		//act.whiteList.emplace_back(ACTION::JUMPING);
		act.blackList.emplace_back("cultist_attack");
		actCtl_.RunInitializeActCtl(type_, "���ړ�", act);
	}

	// �E�������]
	{
		ActModule flipAct;
		flipAct.state = nullptr;
		flipAct.flipFlg = true;
		flipAct.actName = "cultist_idle";
		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("cultist_attack");
		actCtl_.RunInitializeActCtl(type_, "�E����", flipAct);
	}

	// ���������]
	{
		ActModule flipAct;
		flipAct.state = nullptr;
		flipAct.flipFlg = false;
		flipAct.actName = "cultist_idle";

		//flipAct.blackList.emplace_back(ACTION::FALLING);

		flipAct.blackList.emplace_back("cultist_attack");
		actCtl_.RunInitializeActCtl(type_, "������", flipAct);
	}
	// ����
	{
		// checkkeylist�ɗ����Ă���Ԃ̐ݒ���������Ǔ��Ɍ��ʂȂ�
		ActModule act;
		act.actName = "cultist_fall";
		act.state = nullptr;
		//act.checkPoint1 = Vec2{ 0,-10 };			// ����
		//act.checkPoint2 = Vec2{ 0,-10 };			// �E��
		act.checkPoint1 = Vec2{ 0,0 };				// ����
		act.checkPoint2 = Vec2{ 0,0 };				// �E��

		act.checkPoint3 = Vec2{ size.x / 2, size.y / 2 };  // �E��
		act.checkPoint4 = Vec2{ -size.x / 2, size.y / 2 }; // ����

		act.gravity = Vec2{ 0.0f,-5.0f };

		actCtl_.RunInitializeActCtl(type_, "����", act);
	}
	// �U��
	{
		ActModule act;
		act.state = nullptr;
		act.actName = "cultist_attack";
		// ����ݖ�����ׯ�ؽ�(����������ڲ԰�������Ă��U�����ė~�����Ȃ��Ƃ�)
		// fall -> attack�����銴��
		act.blackList.emplace_back("cultist_fall");

		actCtl_.RunInitializeActCtl(type_, "�U��", act);
	}
	// �X�V�֐��̓o�^
	actCtl_.InitUpdater(type_);
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

void Cultist::UseSkill(void)
{

}

void Cultist::Patrol(void)
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
	currentAnimation_ = "cultist_walk";
	auto move = MoveTo::create(0.0f, Vec2(previousPos + speed_.x, getPosition().y));
	//pos_.x += speed_.x;
	//this->setPosition(Vec2(pos_.x, pos_.y));
	this->runAction(move);
	this->runAction(flipFlag_);
	setPosition(Vec2(previousPos + speed_.x, getPosition().y));
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
