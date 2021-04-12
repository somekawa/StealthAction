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
	const auto targetPos = target_.getPosition();
	// ���ޯĂ̻���
	const auto targetDamageCol = target_.GetDamageCol();
	const auto targetAttackCol = target_.GetAttackCol();
	// ���ޯĂƎ��g(������޼ު��)�Ƃ̋���
	const Vec2 distance{ targetDamageCol.origin.x - getPosition().x,targetDamageCol.origin.y - getPosition().y };
	const Vec2 distanceAttack{ targetAttackCol.origin.x - getPosition().x,targetAttackCol.origin.y - getPosition().y };
	// ���ޯĂ̻���/2 + ���g�̻���/2
	const Vec2 diffAttack{ (targetAttackCol.size.width / 2.0f) + (getContentSize().width / 2.0f),((targetAttackCol.size.height / 2.0f) + 5.0f) + (getContentSize().height / 2.0f) };
	const Vec2 diff{ (targetDamageCol.size.width / 2.0f) + (getContentSize().width / 2.0f),((targetDamageCol.size.height/ 2.0f) + 5.0f) + (getContentSize().height / 2.0f) };
	// �������Ă���
	if (abs(distanceAttack.x) <= diffAttack.x && abs(distanceAttack.y) <= diffAttack.y)
	{
		target_.SetIsHitAttack(true);
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
	const auto duration = lpAnimMng.GetAnimationCache(currentAnimation_)->getDuration();
	// �A�j���[�V�����J�E���g���p�����Ԃ𒴂��Ă����
	if (animationFrame_ >= duration)
	{
		// ���[�v�t���O��true�̏ꍇ�̓��[�v�Đ�
		if (lpAnimMng.GetIsLoop(currentAnimation_))
		{
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