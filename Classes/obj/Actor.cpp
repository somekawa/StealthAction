#include "Actor.h"
#include "Scene/GameScene.h"
#include "Gravity.h"

USING_NS_CC;

//Actor::Actor()
//{
//}

Actor::Actor()
{
	deleteFlag_ = false;
	isAnimEnd_ = false;
	animationFrame_ = 0.0f;
	// �������g���Q�Ƃœn���A����
	gravity_ = std::make_unique<Gravity>(*this);
}

Actor::~Actor()
{
}

void Actor::Update(void)
{

	Action();
}

// �A�j���[�V�����̍X�V�֐�(����������Ɖ��ǂ���)
void Actor::UpdateAnimation(float delta)
{
	// �A�j���[�V�����J�E���g�𖈃t���[��delta�l�����Z
	animationFrame_ += delta;
	// ����A�j���[�V�����̏I���܂ł̌p�����Ԃ̊i�[
	auto duration = lpAnimMng.GetAnimationCache(type_, currentAnimation_)->getDuration();
	// �A�j���[�V�����J�E���g���p�����Ԃ𒴂��Ă����
	if (animationFrame_ >= duration)
	{
		// ���[�v�t���O��true�̏ꍇ�̓��[�v�Đ�
		if (lpAnimMng.GetIsLoop(type_, currentAnimation_))
		{
			auto action = RepeatForever::create(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
			this->runAction(action);
			animationFrame_ = 0.0f;
		}
		else
		{
			// false�̏ꍇ��1��̍Đ�
			// 1�A�j���[�V�������I�������t���O�𗧂Ă�
			isAnimEnd_ = true;
		}
	}
}

void Actor::ChangeAnimation(std::string animName)
{
	// ���̓������~�߂�
	this->stopAllActions();
	// �ύX�������A�j���[�V���������݂̃A�j���[�V�����Ɠ����Ȃ�Ή������Ȃ��B
	if (currentAnimation_ == animName)
	{
		return;
	}
	// ���݂̃A�j���[�V������ύX��̃A�j���[�V�������ɕύX
	currentAnimation_ = animName;
	// �A�j���[�V�����I���t���O��false��
	isAnimEnd_ = false;
	// �A�j���[�V�����̃t���[�����������l�ɖ߂�
	animationFrame_ = 0.0f;
}

void Actor::SetPos(Vector2I pos)
{
	pos_ = pos;
}

const int& Actor::GetAnimationFrameInt(void)
{
	// ���t���[�����Z�����l(animationFrame)��1�t���[���ɗv���鎞��(delayPerUnit)������
	// delayPerUnit�Ŋ���ƌ��݂̃t���[���l��int�Ŏ擾�\
	auto delay = lpAnimMng.GetAnimationCache(type_, currentAnimation_)->getDelayPerUnit();
	auto val = (animationFrame_ - delay) / delay;
	return val;
}

