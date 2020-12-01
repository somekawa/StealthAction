#include "obj/Enemy/Enemy.h"
#include "Scene/GameScene.h"
#include "obj/Player.h"
#include "BehaviorBaseAI/BehaviorTree.h"
#include "BehaviorBaseAI/BehaviorData.h"
#include "BehaviorBaseAI/NodeBase.h"
#include "_Debug/_DebugConOut.h"

USING_NS_CC;

Enemy::Enemy(Vector<Node*>& player, BehaviorTree* aiTree):
	player_(player),aiTree_(aiTree),activeNode_(NULL),behaviorData_(NULL)
{
	// �r�w�C�r�A�f�[�^�̐���
	behaviorData_ = new BehaviorData();
	// �U�����Ă��邩�̃t���O�̏�����
	isAttacking_ = false;
	// �A�j���[�V�������ύX���ꂽ���̃t���O�̏�����
	isChangedAnim_ = false;

	isAnimEnd_ = false;
}

Enemy::~Enemy()
{
	//onExit();
}

void Enemy::ChangeDirection(void)
{
	auto playerPos = player_.front()->getPosition();

	if (pos_.x > playerPos.x)
	{
		direction_ = Direction::Left;
	}
	else
	{
		direction_ = Direction::Right;
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
	TRACE("%s", currentAnimation_);
	TRACE("currentAnimation:%s,animFrame:%f\n", currentAnimation_.c_str(), animationFrame_);
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
	auto playerPos = player_.front()->getPosition();

	return abs(playerPos.x - pos_.x);
}

void Enemy::SetIsAttacking(bool flg)
{
	isAttacking_ = flg;
}

void Enemy::AIRun(void)
{
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
