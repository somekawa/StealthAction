#include "obj/Enemy/Enemy.h"
#include "Scene/GameScene.h"
#include "obj/Player.h"
#include "Gravity.h"
#include "ActionRect.h"
#include "obj/Player.h"
#include "HPGauge.h"
#include "BehaviorBaseAI/BehaviorTree.h"
#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"
#include "_Debug/_DebugConOut.h"

USING_NS_CC;

Enemy::Enemy(Vec2 pos,Player& player, BehaviorTree* aiTree,VisionRange visionRange,int hp,Layer& myLayer):
	Actor(hp,myLayer),player_(player),aiTree_(aiTree),activeNode_(NULL),behaviorData_(NULL),visionRange_(visionRange)
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
	//onExit();
}

const float& Enemy::GetVision(void)
{
	// ��ڲ԰���߼޼��
	auto plPos = player_.getPosition();
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
	auto flip = false;
	auto flipBit = 0;
	if (!isMove_[static_cast<int>(direction_)])
	{
		if (direction_ == Direction::Right)
		{
			flipBit = 1;
		}
		else
		{
			flipBit = -1;
		}
		direction_ = (direction_ + flipBit);
		isMove_[static_cast<int>(direction_)] = true;
	}
	else
	{
		if (mType_ == MoveType::Chase)
		{
			// ��ڲ԰���߼޼��
			auto playerPos = player_.getPosition();
			// 1�ڰёO�̎��g�̌����Ă�������̊i�[
			oldDirection_ = direction_;
			// ��ڲ԰�����g�������ɂ���ꍇ
			if (getPosition().x > playerPos.x)
			{
				direction_ = Direction::Left;
			}
			// ��ڲ԰�����g�����E�ɂ���ꍇ
			else
			{
				direction_ = Direction::Right;
			}
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
				direction_ = (Direction)RandomHelper::random_int((int)Direction::Right, (int)Direction::Left);
			}
		}
	}

	if (direction_ == Direction::Left)
	{
		flip = false;
	}
	else if (direction_ == Direction::Right)
	{
		flip = true;
	}
	// patrol(����)�s���łȂ��ꍇ��patrolFrame���۸�
	//if (mType_ != MoveType::Patrol)
	//{
	//	patrolFrame_ = 0.0f;
	//}
	runAction(FlipX::create(flip));
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
			// ��Ұ�ޔ���ײ�ް�ޯ����������
			if (plCol->GetData().type_ == 1)
			{
				// �����̺ײ�ް�ޯ���̑���
				for (auto myCol : currentCol_)
				{
					// �U������ײ�ް�ޯ����������
					if (myCol->GetData().type_ == 0)
					{
						// ��ڲ԰�̓����蔻��Ώۂ��ޯ������Ұ�޺ײ�ް�ޯ���ް����i�[
						auto plDamageCol = plCol->GetData();
						// �����̓����蔻��̺ײ�ް�ޯ���ް���attack�ײ�ް�ޯ���ް����i�[
						auto attackColData = myCol->GetData();
						// �����ɂ���ċ�`�߼޼�݂��ύX����̂ŁA�ύX�����߼޼�݂ɉ�����begin��ύX
						attackColData.begin_ = Vector2I(myCol->getPosition().x,myCol->getPosition().y);

						auto attackColPos = Vec2{ getPosition().x + (attackColData.begin_.x + (attackColData.size_.x/2)),
												  getPosition().y + (attackColData.begin_.y + (attackColData.size_.y/2)) };
						auto damageColPos = Vec2{ player_.getPosition().x + (plDamageCol.begin_.x + (plDamageCol.size_.x/2)) ,
												  player_.getPosition().y + (plDamageCol.begin_.y + (plDamageCol.size_.y/2)) };
						// �����蔻����{
						if (abs(attackColPos.x - damageColPos.x) <= visionRange_.attack_ + 10.0f &&
							abs(attackColPos.y - damageColPos.y) <= 80.0f)
						{
							auto t = type_;
							// �U���𓖂Ă�trigger��true�ɂ���
							hittingToPlayer_ = true;
							return true;
						}
					}
				}
			}
		}
	}
	// �U�����������Ă��Ȃ��ꍇ�������Ȃ�
	return false;
}

void Enemy::CheckHitPLAttack(void)
{
	if (!onDamaged_)
	{
		// ��ڲ԰�̌��݂̺ؼޮ��ް�
		auto plColData = player_.GetCurrentCol();
		// ��ڲ԰�̌��݂̺ؼޮ��ް��ŉ�
		for (auto attackCol : plColData)
		{
			// ���݂���ڲ԰�̺ؼޮ��ް��̒��ɍU����`���L���
			if (attackCol->GetData().type_ == 0)
			{
				// ���݂̎����̺ؼޮ��ް��ŉ�
				for (auto myCol : currentCol_)
				{
					// ���݂̎����̺ؼޮ��ް��̒�����Ұ�ދ�`���L���
					if (myCol->GetData().type_ == 1)
					{
						// ��ڲ԰���߼޼�݂����݂ǂ����s��
						auto plPos = player_.getPosition();
						// �U����`���߼޼��(�^�񒆂ɐݒ�)
						auto attackColPos = Vec2{ player_.getPosition().x + attackCol->GetData().begin_.x + (attackCol->GetData().size_.x / 2),
												  player_.getPosition().y + attackCol->GetData().begin_.y + (attackCol->GetData().size_.y / 2) };
						// ��Ұ�ދ�`���߼޼��(�^�񒆂ɐݒ�)
						auto damageColPos = Vec2{ getPosition().x + myCol->GetData().begin_.x + (myCol->GetData().size_.x / 2),
												  getPosition().y + myCol->GetData().begin_.y + (myCol->GetData().size_.y / 2) };
						// �U����`����Ұ�ދ�`�̋�����50�ȉ����Ɠ������Ă��锻���
						if (abs(attackColPos.x - damageColPos.x) <= 60.0f &&
							abs(attackColPos.y - damageColPos.y) <= 80.0f)
						{
							hp_ -= 10;
							// HP�����̃e�X�g�R�[�h
							//auto a = ((Game*)Director::getInstance()->getRunningScene());
							//auto b = (EnemyHPGauge*)a->getChildByTag((int)zOlder::FRONT)->getChildByName(myName_ + "_" + std::to_string(id_) + "_HP");
							//b->SetHP(hp_);	// -10�Ȃǂ̃_���[�W�ʂ͓G�̍U���͂ɕς���΂���
							// onDamaged_��true��
							OnDamaged();
						}
					}
				}
			}
		}
	}
}

const float& Enemy::GetDistance(void)
{
	// ��ڲ԰���߼޼��
	auto playerPos = player_.getPosition();
	// ��ڲ԰���߼޼�݂Ǝ��g���߼޼�݂�����������abs����������Ԃ�
	return abs(playerPos.x - getPosition().x);
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

bool Enemy::CheckObjHit(void)
{
	auto director = Director::getInstance();
	auto a = director->getRunningScene()->getChildByTag((int)zOlder::BG);
	if ((TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData") == nullptr)
	{
		return false;
	}
	const int chipSize = 48;
	auto CollisionData = (TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData")->getChildByName("col");
	auto ColSize = CollisionData->getLayerSize();

	auto position = this->getPosition();

	auto nextPos = [&]() {
		Vec2 next;
		switch (direction_)
		{
		case Direction::Right:
			next = { position.x + (this->getContentSize().width / 2) + speed_.x,position.y };
			break;
		case Direction::Left:
			next = { position.x - (this->getContentSize().width / 2) + speed_.x,position.y };

			break;
		case Direction::Max:
			break;
		default:
			break;
		}
		return next;
	};

	auto grid = Vec2{ nextPos().x,nextPos().y } / chipSize;

	auto gridPos = Vec2(grid.x, ColSize.height - grid.y);

	// �͈͊Ocheck
	if (gridPos.x > ColSize.width || gridPos.x < 0 ||
		gridPos.y > ColSize.height || gridPos.y < 0)
	{
		return true;
	}
	auto gridGid = CollisionData->getTileGIDAt(gridPos);
	if (gridGid != 0)
	{
		//sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - (module.flame));
		return true;
	}

	return false;
}

void Enemy::Hit(void)
{
	TRACE("Hit!\n");
	auto director = Director::getInstance();
	auto scene = director->getRunningScene();
	auto hpGauge = (HPGauge*)scene->getChildByTag((int)zOlder::FRONT)->getChildByName(getName() + "_HPgauge" + "_" + std::to_string(id_));
	if (!onDamaged_)
	{
		hp_ -= 10.0f;
		hpGauge->SetHP(hp_);
		onDamaged_ = true;
	}
	if (isAnimEnd_)
	{
		onDamaged_ = false;
		isHitAttack_ = false;
	}
}

void Enemy::Death(void)
{
	TRACE("death");
	// deatthӰ��݂��I�����Ă�����
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
	if (isHitAttack_)
	{
		stateTransitioner_ = &Enemy::Hit;
	}
	//AIRun();
	if (isAnimEnd_)
	{
		// �܂���Ұ�ނ�^������悤�ɂ��邽�߂�false�ɂ��Ă���
		onDamaged_ = false;
		currentAnimation_ = myName_ + "_idle";
		//stateTransitioner_ = &Enemy::Idle;
	}
	//currentAnimation_ = myName_ + "_idle";
}
