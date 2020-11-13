#include "Actor.h"
#include "Scene/GameScene.h"

USING_NS_CC;

//Actor::Actor()
//{
//}

Actor::Actor()
{
	deleteFlag_ = false;
	isAnimEnd_ = false;
	animationFrame_ = 0.0f;
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

