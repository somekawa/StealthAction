#include "obj/Enemy/Enemy.h"
#include "Scene/GameScene.h"
#include "obj/Player.h"
#include "Gravity.h"
#include "ActionRect.h"
#include "obj/Player.h"
#include "BehaviorBaseAI/BehaviorTree.h"
#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"
#include "_Debug/_DebugConOut.h"

USING_NS_CC;

Enemy::Enemy(Player& player, BehaviorTree* aiTree,VisionRange visionRange,int hp):
	Actor(hp),player_(player),aiTree_(aiTree),activeNode_(NULL),behaviorData_(NULL),visionRange_(visionRange)
{
	// �r�w�C�r�A�f�[�^�̐���
	behaviorData_ = new BehaviorData();

	// �A�j���[�V�������ύX���ꂽ���̃t���O�̏�����
	isChangedAnim_ = false;
	// �v���C���[�����F�������̃t���O�̏�����
	identifiedPlayer_ = false;
	// �����ύX�t���O�̏�����
	isChangeDir_ = false;
	isAnimEnd_ = false;
	isMoveComplete_ = true;
	// �����̍U�������������t���O�̏�����
	hittingToPlayer_ = false;

	oldPos_ = getPosition();
	// �v���C���[�Ǝ����̋����𑪂��ăC���X�^���X
	vision_ = abs(player_.getPosition().x - getPosition().x);
}

Enemy::~Enemy()
{
	//onExit();
}

const float& Enemy::GetVision(void)
{
	auto plPos = player_.getPosition();

	vision_ = abs(plPos.x - getPosition().x);
	return vision_;
}

void Enemy::ChangeDirection(void)
{
	auto playerPos = player_.getPosition();

	if (getPosition().x > playerPos.x)
	{
		direction_ = Direction::Left;
	}
	else
	{
		direction_ = Direction::Right;
	}
}

bool Enemy::OnAttacked(void)
{
	// ��ڲ԰�ɍU����1��̂ݓ��Ă����̂�
	// �U����1�񓖂���Γ����蔻��͐؂�
	if (!hittingToPlayer_)
	{
		// ��ڲ԰�̺ײ�ް�ޯ���̑���
		for (auto plCol : player_.GetCurrectCol())
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

						auto attackColPos = Vec2{ getPosition().x + (attackColData.begin_.x + (attackColData.size_.x / 2)),
												  getPosition().y + (attackColData.begin_.y + (attackColData.size_.y / 2)) };
						auto damageColPos = Vec2{ player_.getPosition().x + (plDamageCol.begin_.x + (plDamageCol.size_.x / 2)) ,
												  player_.getPosition().y + (plDamageCol.begin_.y + (plDamageCol.size_.y / 2)) };
						// �����蔻����{
						if (abs(attackColPos.x - damageColPos.x) <= 50.0 &&
							abs(attackColPos.y - damageColPos.y) <= 50.0)
						{
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
	// ��ڲ԰�̌��݂̺ؼޮ��ް�
	auto plColData = player_.GetCurrectCol();
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
					auto attackColPos = Vec2{ player_.getPosition().x + attackCol->GetData().begin_.x + (attackCol->GetData().size_.x/2),
											  player_.getPosition().y + attackCol->GetData().begin_.y + (attackCol->GetData().size_.y/2)};
					// ��Ұ�ދ�`���߼޼��(�^�񒆂ɐݒ�)
					auto damageColPos = Vec2{ getPosition().x + myCol->GetData().begin_.x + (myCol->GetData().size_.x/2),
											  getPosition().y + myCol->GetData().begin_.y + (myCol->GetData().size_.y/2) };
					// �U����`����Ұ�ދ�`�̋�����50�ȉ����Ɠ������Ă��锻���
					if (abs(attackColPos.x - damageColPos.x) <= 50.0 &&
						abs(attackColPos.y - damageColPos.y) <= 50.0)
					{
						// onDamaged_��true��
						OnDamaged();
					}
				}
			}
		}
	}
}

// �A�j���[�V�����̍X�V�֐�(����������Ɖ��ǂ���)
void Enemy::UpdateAnimation(float delta)
{
	/*if (currentAnimation_ != previousAnimation_)
	{
		animationFrame_ = lpAnimMng.GetFrameNum(type_,currentAnimation_);
	}*/
	// �A�j���[�V�����J�E���g�𖈃t���[��delta�l�����Z
	animationFrame_ -= delta * 10;
	//TRACE("%s", currentAnimation_);
	//TRACE("currentAnimation:%s,animFrame:%f\n", currentAnimation_.c_str(), animationFrame_);
	// ����A�j���[�V�����̏I���܂ł̌p�����Ԃ̊i�[
	auto duration = lpAnimMng.GetAnimationCache(type_, currentAnimation_)->getDuration();
	// �A�j���[�V�����J�E���g���p�����Ԃ𒴂��Ă����
	if (animationFrame_ <= 0.0f)
	{
		// ���[�v�t���O��true�̏ꍇ�̓��[�v�Đ�
		if (lpAnimMng.GetIsLoop(type_, currentAnimation_))
		{
			//auto action = RepeatForever::create(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
			//this->runAction(action);
			animationFrame_ = lpAnimMng.GetFrameNum(type_, currentAnimation_);
			//animationFrame_ = 0.0f;
		}
		else
		{
			// false�̏ꍇ��1��̍Đ�
			// 1�A�j���[�V�������I�������t���O�𗧂Ă�
			isAnimEnd_ = true;
			//animationFrame_ = 0.0f;
		}
	}
	TRACE("animEndFlag:%d\n", isAnimEnd_);

}

void Enemy::ChangeAnimation(std::string animName)
{
	// ���̓������~�߂�
	this->stopAllActions();

	// �ύX�������A�j���[�V���������݂̃A�j���[�V�����Ɠ����Ȃ�Ή������Ȃ��B
	/*if (currentAnimation_ == animName)
	{
		return;
	}*/
	// ���݂̃A�j���[�V������ύX��̃A�j���[�V�������ɕύX
	currentAnimation_ = animName;
	animationFrame_ = lpAnimMng.GetFrameNum(type_, currentAnimation_);

	// �A�j���[�V�����I���t���O��false��
	isAnimEnd_ = false;
	// �A�j���[�V�����̃t���[�����������l�ɖ߂�
	//animationFrame_ = 0.0f;
	if (lpAnimMng.GetIsLoop(type_, currentAnimation_))
	{
		auto action = RepeatForever::create(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
		this->runAction(action);
	}
	else
	{
		this->runAction(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
	}
}

const float& Enemy::DistanceCalcurator(void)
{
	auto playerPos = player_.getPosition();

	return abs(playerPos.x - getPosition().x);
}

void Enemy::AIRun(void)
{
	// �_���[�W���炢
	// �_���[�W���������true�ɂȂ�
	onDamaged_ = CheckHitAttack(player_.GetAttackRect());
	// �_���[�W�e�X�g
	if (onDamaged_)
	{
		int i = 0;
	}

	if (activeNode_ == NULL)
	{
		activeNode_ = aiTree_->Inference(this, behaviorData_);
	}

	if (activeNode_ != NULL)
	{
		activeNode_ = aiTree_->Run(this, activeNode_, behaviorData_);
	}
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
		case Direction::Up:
			break;
		case Direction::Down:
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
