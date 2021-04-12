#include "_Debug/_DebugConOut.h"
#include "Scene/GameScene.h"
#include "obj/Player.h"
#include "Gravity.h"
#include "ActionRect.h"
#include "../../HPGauge.h"
#include "BehaviorBaseAI/BehaviorTree.h"
#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"
#include "Enemy.h"

USING_NS_CC;

Enemy::Enemy(Vec2 pos,Player& player, BehaviorTree* aiTree,VisionRange visionRange,int hp,Layer& myLayer):
	Actor(hp,myLayer),player_(player),aiTree_(aiTree),activeNode_(nullptr),behaviorData_(nullptr),visionRange_(visionRange)
{
	retain();
	// �r�w�C�r�A�f�[�^�̐���
	behaviorData_ = new BehaviorData();
	setPosition(pos);
	// �A�j���[�V�������ύX���ꂽ���̃t���O�̏�����
	isChangedAnim_ = false;
	isAttacking_ = false;
	// �v���C���[�����F�������̃t���O�̏�����
	identifiedPlayer_ = false;
	// �����ύX�t���O�̏�����
	isChangeDir_ = false;
	isAnimEnd_ = false;
	isMoveComplete_ = true;
	// �����̍U�������������t���O�̏�����
	hittingToPlayer_ = false;
	// �����邩���׸ނ̏�����
	for (auto& flg : isMove_)
	{
		flg = true;
	}
	// ��ԑJ�ڂ̊֐��߲���̏�����
	stateTransitioner_ = &Enemy::Idle;
	// moveType�̏�����
	mType_ = MoveType::Non;
	oldPos_ = getPosition();
	// patrol(����s��)�����Ă����ڰѐ��̏�����
	patrolFrame_ = 0.0f;
	// �v���C���[�Ǝ����̋����𑪂��ăC���X�^���X
	vision_ = abs(player_.getPosition().x - getPosition().x);
}

Enemy::~Enemy()
{
}

const float& Enemy::GetVision(void)
{
	// ��ڲ԰���߼޼��
	const Vec2 plPos = player_.getPosition();
	// ���݂̓G�̎��o���(��ڲ԰�������ɑ΂��Ăǂ̂��炢�̏��ɂ��邩)
	vision_ = abs(plPos.x - getPosition().x);
	return vision_;
}

void Enemy::DeleteSelfOnFloor(void)
{
	if (Director::getInstance()->getRunningScene()->getName() != "GameScene")
	{
		removeFromParentAndCleanup(true);
		return;
	}
}

void Enemy::ChangeDirection(float delta)
{
	int flipBit = 0;
	if (!isMove_[static_cast<int>(direction_)])
	{
		direction_ == Direction::Right ? flipBit = 1 : flipBit = -1;
		direction_ = (direction_ + flipBit);
		isMove_[static_cast<int>(direction_)] = true;
	}
	else
	{
		if (mType_ == MoveType::Chase)
		{
			// ��ڲ԰���߼޼��
			const Vec2 playerPos = player_.getPosition();
			// 1�ڰёO�̎��g�̌����Ă�������̊i�[
			oldDirection_ = direction_;
			// ��ڲ԰�����g�����ǂ���ɂ��邩�ŕ��������߂�
			direction_ = (getPosition().x > playerPos.x ? Direction::Left : Direction::Right);
		}
		else if (mType_ == MoveType::Patrol)
		{
			// 1�ڰёO�̎��g�̌����Ă�������̊i�[
			oldDirection_ = direction_;
			// patrol�����Ă���Ԃ̶��Ă����Z
			patrolFrame_ += delta;
			// patrol(����)�s�������Ă���̂����Ԋu�ȏゾ������
			if (patrolFrame_ >= DefPatrolFrame)
			{
				patrolFrame_ = 0.0f;
				// �E�����̕���������тŌ��肷��
				// cocos2d::RandomHelper::random_int(int min,int max);
				// min(�ŏ�)�`max(�ő�)�̊Ԃ�����тȐ��l��Ԃ�
				direction_ = (Direction)RandomHelper::random_int(static_cast<int>(Direction::Right), static_cast<int>(Direction::Left));
			}
		}
		else
		{
			// �����������s��Ȃ�
		}
	}

	runAction(FlipX::create(direction_ == Direction::Left ? false : true));
}

bool Enemy::OnAttacked(void)
{
	// ��ڲ԰�ɍU����1��̂ݓ��Ă����̂�
	// �U����1�񓖂���Γ����蔻��͐؂�
	if (!hittingToPlayer_)
	{
		// ��ڲ԰�̺ײ�ް�ޯ���̑���
		for (auto plCol : player_.GetCurrentCol())
		{
			if (plCol->GetData().type_ != 1)
			{
				// ��Ұ�ޔ���ײ�ް�ޯ���ȊO�Ȃ牽�����Ȃ�
				continue;
			}
			// �����̺ײ�ް�ޯ���̑���
			for (auto myCol : currentCol_)
			{
				if (myCol->GetData().type_ != 0)
				{
					// �U������ײ�ް�ޯ���ȊO�Ȃ牽�����Ȃ�
					continue;
				}
				// ��ڲ԰�̓����蔻��Ώۂ��ޯ������Ұ�޺ײ�ް�ޯ���ް����i�[
				const auto plDamageCol = plCol->GetData();
				// �����̓����蔻��̺ײ�ް�ޯ���ް���attack�ײ�ް�ޯ���ް����i�[
				auto attackColData = myCol->GetData();
				// �����ɂ���ċ�`�߼޼�݂��ύX����̂ŁA�ύX�����߼޼�݂ɉ�����begin��ύX
				attackColData.begin_ = Vector2I(static_cast<int>(myCol->getPosition().x),static_cast<int>(myCol->getPosition().y));

				const Vec2 attackColPos{ getPosition().x + (attackColData.begin_.x + (attackColData.size_.x/2.0f)),
										 getPosition().y + (attackColData.begin_.y + (attackColData.size_.y/2.0f)) };
				const Vec2 damageColPos{ player_.getPosition().x + (plDamageCol.begin_.x + (plDamageCol.size_.x/2.0f)) ,
										 player_.getPosition().y + (plDamageCol.begin_.y + (plDamageCol.size_.y/2.0f)) };
				// �����蔻����{
				if (abs(attackColPos.x - damageColPos.x) <= visionRange_.attack_ + 10.0f &&
					abs(attackColPos.y - damageColPos.y) <= 80.0f)
				{
					// �U���𓖂Ă�trigger��true�ɂ���
					hittingToPlayer_ = true;
					return true;
				}
			}
		}
	}
	// �U�����������Ă��Ȃ��ꍇ�������Ȃ�
	return false;
}

const float Enemy::GetDistance(void)
{
	// ��ڲ԰���߼޼�݂Ǝ��g���߼޼�݂�����������abs����������Ԃ�
	return abs(player_.getPosition().x - getPosition().x);
}

void Enemy::SetAlive(bool flg)
{
	isAlive_ = flg;
}

void Enemy::SetID(int id)
{
	id_ = id;
}

void Enemy::AIRun(void)
{
	// activeState��NULL�̏ꍇ��behaviorTree���琄�_���AaiTree�ɍs�����i�[
	if (activeNode_ == NULL)
	{
		activeNode_ = aiTree_->Inference(this, behaviorData_);
	}
	// activeNode��NULL�łȂ��ꍇ�͓G�̍s���Ƃ��Ēǉ������m�[�h
	// ��������ɍ��v����s�������s
	if (activeNode_ != NULL)
	{
		activeNode_ = aiTree_->Run(this, activeNode_, behaviorData_);
	}
}

void Enemy::SetMove(const Direction& dir,bool flg)
{
	isMove_[static_cast<int>(dir)] = flg;
}

void Enemy::SetMoveType(MoveType type)
{
	mType_ = type;
}

void Enemy::Hit(void)
{
	TRACE("Hit!\n");
	const auto director = Director::getInstance();
	const auto scene = director->getRunningScene();
	const auto hpGauge = (HPGauge*)scene->getChildByTag(static_cast<int>(zOrder::FRONT))->getChildByName(getName() + "_HPgauge" + "_" + std::to_string(id_));

	if (currentAnimation_ == getName() + "_hit")
	{
		if (previousAnimation_ == currentAnimation_)
		{
			return;
		}
	}

	if (isHitAttack_)
	{
		const auto player = scene->getChildByTag(static_cast<int>(zOrder::CHAR_PL))->getChildByName("player1");
		hp_ -= static_cast<Player*>(player)->GetGiveDamageNum();
		hpGauge->SetHP(static_cast<float>(hp_));
		isHitAttack_ = false;
	}
}

void Enemy::Death(void)
{
	TRACE("death");
	// deathӰ��݂��I�����Ă�����
	if (isAnimEnd_)
	{
		// ���g���������邽�߂Ɏ����̖��O��death�ɂ���(���ł������Ă���)
		setName(myName_ + "_death");
		// �����Ă����׸ނ�false��(���񂾎��ɂ���)
		isAlive_ = false;
	}
}

void Enemy::Idle(void)
{
	if (isHitAttack_ && (previousAnimation_ != currentAnimation_))
	{
		stateTransitioner_ = &Enemy::Hit;
	}
	if (isAnimEnd_)
	{
		// �܂���Ұ�ނ�^������悤�ɂ��邽�߂�false�ɂ��Ă���
		isHitAttack_ = false;
		currentAnimation_ = myName_ + "_idle";
	}
}