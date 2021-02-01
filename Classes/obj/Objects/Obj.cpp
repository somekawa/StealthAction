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
	auto targetDamageCol = target_.GetDamageCol();
	auto targetAttackCol = target_.GetAttackCol();
	// ���ޯĂƎ��g(������޼ު��)�Ƃ̋���
	auto distance = Vec2{ targetDamageCol.origin.x - getPosition().x,targetDamageCol.origin.y - getPosition().y };
	auto distanceAttack = Vec2{ targetAttackCol.origin.x - getPosition().x,targetAttackCol.origin.y - getPosition().y };
	// ���ޯĂ̻���/2 + ���g�̻���/2
	auto diffAttack = Vec2{ (targetAttackCol.size.width / 2) + (getContentSize().width / 2),((targetAttackCol.size.height / 2) + 5.0f) + (getContentSize().height / 2) };
	auto diff = Vec2{ (targetDamageCol.size.width / 2) + (getContentSize().width / 2),((targetDamageCol.size.height/ 2) + 5.0f) + (getContentSize().height / 2) };
	// �������Ă���
	if (abs(distanceAttack.x) <= diffAttack.x && abs(distanceAttack.y) <= diffAttack.y)
	{
		//target_.SetIsHitAttack(true);
		// ���������׸ނ�true��
		isHitTarget_ = true;
		return true;
	}
	// �������Ă���
	if (abs(distance.x) <= diff.x && abs(distance.y) <= diff.y)
	{
		target_.SetIsHitAttack(true);
		// ���������׸ނ�true��
		isHitTarget_ = true;
		return true;
	}
	// �������Ă��Ȃ�
	return false;
}

void Obj::UpdateAnimation(float delta)
{
// �A�j���[�V�����J�E���g�𖈃t���[��delta�l�����Z
	animationFrame_ += delta;
	// ����A�j���[�V�����̏I���܂ł̌p�����Ԃ̊i�[
	auto duration = lpAnimMng.GetAnimationCache(currentAnimation_)->getDuration();
	// �A�j���[�V�����J�E���g���p�����Ԃ𒴂��Ă����
	if (animationFrame_ >= duration)
	{
		// ���[�v�t���O��true�̏ꍇ�̓��[�v�Đ�
		if (lpAnimMng.GetIsLoop(currentAnimation_))
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
	if (lpAnimMng.GetIsLoop(currentAnimation_))
	{
		auto action = RepeatForever::create(Animate::create(lpAnimMng.GetAnimationCache(currentAnimation_)));
		this->runAction(action);
	}
	else
	{
		this->runAction(Animate::create(lpAnimMng.GetAnimationCache(currentAnimation_)));
	}
}
