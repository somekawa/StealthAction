#pragma once

#include <cocos2d.h> 
#include <memory>
#include <list>
#include "Geometory.h"
#include "Direction/Direction.h"
#include "ActorType.h"
#include "anim/ActionCtl.h"
#include "input/OPRT_state.h"
#include "module/ActModule.h"

class Input;
class Collider;
class ActionRect;
class Gravity;

// �U���̋�`
struct AttackRect
{
	bool offsetFlag_ = false;
	// �|�W�V����
	cocos2d::Vec2 offset_ = {0.0f,0.0f};
	// �T�C�Y
	cocos2d::Size size_;
};

#define FrameConverter(X) (X / 1000)

class Actor :
	public cocos2d::Sprite
{
public:
	// �f�t�H���g�R���X�g���N�^�ǉ���
	//Actor();
	Actor(int hp,cocos2d::Layer& myLayer);
	~Actor();
	// �eActor�̍s������
	virtual void Action(void) = 0;
	// �X�V����
	virtual void Update(void);
	// �X�V
	virtual void update(float delta) = 0;

	// ���݂̃A�j���[�V�������擾
	virtual std::string GetAction(void);
	// currentAnimation�̾��
	virtual void SetAction(std::string actName);

	// �����ύX
	virtual void ChangeDirection(void) = 0;
	// �A�j���[�V�����o�^
	virtual void AnimRegistrator(void) = 0;
	// �I�u�W�F�N�g�����񂾂�deleteFlag��true�ɂ���
	virtual void Delete(void)
	{
		deleteFlag_ = true;
	}
	// deleteFlag��true�ɂȂ��Ă���I�u�W�F�N�g�������Alist����������邽�߂Ɏg�p
	virtual const bool GetDeleteFlag(void)
	{
		return deleteFlag_;
	}
	// �߼޼�ݎ擾
	virtual const Vector2I& GetPos(void) const
	{
		return pos_;
	}

	// �U����`�̎擾
	virtual const AttackRect& GetAttackRect(void)
	{
		return attackRect_;
	}
	// ��Ұ��݂��ڰђl�̎擾
	virtual const float& GetAnimationFrame(void)
	{
		return animationFrame_;
	}
	// �߼޼�ݾ��
	virtual void SetPos(Vector2I pos);
	// �����̎擾
	virtual const Direction& GetDirection(void)
	{
		return direction_;
	}
	// �����Ă�������̃Z�b�g
	virtual void SetDirection(Direction dir);

	// ������K�v����̂�������Ȃ����A�F���g���Ǝv���A�쐬���܂���
	// �����l�Ō��݂̃t���[���l���擾
	virtual const int& GetAnimationFrameInt(void);

	virtual void CheckMapObjHit(float delta);
	// �ǂɓ������Ă��邩����
	virtual const bool& IsHitWall(void)
	{
		return isHitWall_;
	}
	// ���ɐڒn���Ă��邩�̔���
	virtual const bool& OnFloor(void)
	{
		return onFloor_;
	}
	// �_���[�W����������Ƃ��Ɏg�p
	virtual void OnDamaged(void)
	{
		onDamaged_ = true;
	}
	void SetIsAttacking(bool flg);
	// �U�����Ă��邩�̃t���O�擾�֐�
	virtual const bool& IsAttacking(void)
	{
		return isAttacking_;
	}
	// ��Ұ�ނ���������׸ނ̎擾�֐�
	virtual const bool& GetOnDamaged(void)
	{
		return onDamaged_;
	}
	// �U����`�̵̾�Ă�ݒ�
	virtual void SetAttackOffset(cocos2d::Vec2 offset);
	// ���݂̃R���C�_�[�{�b�N�X�̎擾
	const std::vector <std::shared_ptr<ActionRect>>& GetCurrectCol(void)
	{
		return currentCol_;
	}
	// HP�̎擾
	const int& GetHp(void)
	{
		return hp_;
	}
	// ���������ߎ擾
	virtual const ActorType& GetType(void)
	{
		return type_;
	}
	// �U����޼ު�Ă𔭎˂��Ă��邩�̎擾
	virtual bool GetFire(void)
	{
		return isFire_;
	}
	// �����Ă��邩���׸ގ擾
	virtual bool AliveCheck(void)
	{
		return isAlive_;
	}
	// ���W���[���̓o�^
	virtual  void actModuleRegistration(void) = 0;							

private:

protected:
	// hp
	int hp_;
	// �|�W�V����
	Vector2I pos_;
	// ���g�̃T�C�Y
	Vector2I size_;
	// �X�s�[�h
	Vector2I speed_;
	// �L�����N�^�[�̃^�C�v
	ActorType type_;
	// ���݂̱�Ұ���
	std::string currentAnimation_;
	// 1�t���[���O�̃A�j���[�V����
	std::string previousAnimation_;
	// ��Ұ��݂��Ă��鎞�͂����ڰѕϐ������Z
	float animationFrame_;
	// �R���C�_�[�{�b�N�X�̋�`�\����A�f�[�^��z�񂩂���o�����߁A
	// ��L�̃A�j���[�V�����t���[����int�^�����ɒ�����frame�l
	int animationFrame_int_;
	// �A�j���[�V�������I�����Ă��邩�ǂ����̃t���O
	bool isAnimEnd_;

	// �摜���]�̃t���O
	cocos2d::FlipX* flipFlag_;

	// �����Ă������
	Direction direction_;

	// �I�u�W�F�N�g�������t���O
	bool deleteFlag_;
	// �ڒn���Ă��邩�̃t���O
	bool onFloor_;
	// �ǂɓ����������̃t���O
	bool isHitWall_;
	// �����̺ײ�ް�ް�
	std::unordered_map<std::string,
		std::vector<std::vector<std::shared_ptr<ActionRect>>>>collider_;
	// ���݂̃R���C�_�[
	std::vector <std::shared_ptr<ActionRect>> currentCol_;
	// �d��
	std::unique_ptr<Gravity> gravity_;
	// �_���[�W��H������t���O
	bool onDamaged_;
	// �U�����Ă��邩�̃t���O
	bool isAttacking_;
	// �U����`
	AttackRect attackRect_;
	// �������g�����݂���ڲ԰
	cocos2d::Layer& myLayer_;

	cocos2d::Layer* attackLayer_;
	// �U����޼ު�Ă��o���Ă��邩���׸�
	bool isFire_;
	// �����Ă��邩����ł��邩���׸�
	bool isAlive_;

	// ActionCtl���Ăяo��
	ActionCtl actCtl_ = ActionCtl();							
};