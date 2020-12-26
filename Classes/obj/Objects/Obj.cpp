#include "Obj.h"
#include "anim/AnimMng.h"
#include "_Debug/_DebugConOut.h"

USING_NS_CC;

Obj::Obj(Actor& target):
	target_(target)
{
	animationFrame_ = 0.0f;
	isAnimEnd_ = false;
	isHitTarget_ = false;
}

Obj::~Obj()
{
}

void Obj::update(float delta)
{
}

bool Obj::InitAnim(void)
{
	// ��Ұ��݂̓o�^
	AnimRegistrator();
	return true;
}

bool Obj::HitToTarget(void)
{
	// ���ޯĂ��߼޼��
	auto targetPos = target_.getPosition();
	// ���ޯĂ̻���
	auto targetSize = target_.getContentSize();
	// ���ޯĂƎ��g(������޼ު��)�Ƃ̋���
	auto distance = Vec2{ targetPos.x - getPosition().x,targetPos.y - getPosition().y };
	// ���ޯĂ̻���/2 + ���g�̻���/2
	auto diff = Vec2{ (targetSize.width / 2) + (getContentSize().width / 2),(targetSize.height / 2) + (getContentSize().height / 2) };
	// �������Ă���
	if (abs(distance.x) <= diff.x && abs(distance.y) <= diff.y)
	{
		// ���������׸ނ�true��
		isHitTarget_ = true;
		return true;
	}
	// �������Ă��Ȃ�
	return false;
}

void Obj::UpdateAnimation(float delta)
{
	/*if (currentAnimation_ != previousAnimation_)
{
	animationFrame_ = lpAnimMng.GetFrameNum(type_,currentAnimation_);
}*/
// �A�j���[�V�����J�E���g�𖈃t���[��delta�l�����Z
	animationFrame_ += delta;
	//TRACE("%s", currentAnimation_);
	//TRACE("currentAnimation:%s,animFrame:%f\n", currentAnimation_.c_str(), animationFrame_);
	if (currentAnimation_ == "impact")
	{
		int i = 0;
	}
	// ����A�j���[�V�����̏I���܂ł̌p�����Ԃ̊i�[
	auto duration = lpAnimMng.GetAnimationCache(type_, currentAnimation_)->getDuration();
	// �A�j���[�V�����J�E���g���p�����Ԃ𒴂��Ă����
	if (animationFrame_ >= duration)
	{
		// ���[�v�t���O��true�̏ꍇ�̓��[�v�Đ�
		if (lpAnimMng.GetIsLoop(type_, currentAnimation_))
		{
			//auto action = RepeatForever::create(Animate::create(lpAnimMng.GetAnimationCache(type_, currentAnimation_)));
			//this->runAction(action);
			animationFrame_ = 0.0f;
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

void Obj::ChangeAnimation(std::string animName)
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
	animationFrame_ = 0.0f;

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
