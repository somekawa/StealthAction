#pragma once
#include <cocos2d.h> 
#include <memory>
#include <list>
#include <vector>
#include <string>
#include "Geometory.h"
#include "Direction/Direction.h"
#include "ActorType.h"
#include "anim/ActionCtl.h"
#include "input/OPRT_state.h"
#include "module/ActModule.h"
#include "../Loader/FileLoder.h"

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
	Actor(int hp,cocos2d::Layer& myLayer);
	~Actor();
	// �X�V����
	virtual void Update(void);
	// �X�V
	virtual void update(float delta) = 0;
	// �A�j���[�V�����̍X�V
	virtual void UpdateAnimation(float delta);
	// �A�j���[�V�����̕ύX
	// param@ animName: �ύX��A�j���[�V������
	virtual void ChangeAnimation(std::string animName);
	// ���݂̃A�j���[�V�������擾
	virtual std::string GetAction(void);
	// currentAnimation�̃Z�b�g
	virtual void SetAction(std::string actName);
	// �A�j���[�V�����o�^
	virtual void AnimRegistrator(void);
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
	// �|�W�V�����擾
	virtual const Vector2I& GetPos(void) const
	{
		return pos_;
	}
	// �U����`�̎擾
	virtual const AttackRect& GetAttackRect(void)
	{
		return attackRect_;
	}
	// �A�j���[�V�����̃t���[���l�̎擾
	virtual const float& GetAnimationFrame(void)
	{
		return animationFrame_;
	}
	// �|�W�V�����Z�b�g
	virtual void SetPos(Vector2I pos);
	// �����̎擾
	virtual const Direction& GetDirection(void)
	{
		return direction_;
	}
	// �����Ă�������̃Z�b�g
	virtual void SetDirection(Direction dir);
	// �����l�Ō��݂̃t���[���l���擾
	virtual const int GetAnimationFrameInt(void);
	virtual const int GetAnimationFrameInt(std::string str);
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
	virtual void OnIsHitAttack(void)
	{
		isHitAttack_ = true;
	}
	void SetIsAttacking(bool flg);
	// �U�����Ă��邩�̃t���O�擾�֐�
	virtual const bool& IsAttacking(void)
	{
		return isAttacking_;
	}
	// �U����`�̃I�t�Z�b�g��ݒ�
	virtual void SetAttackOffset(cocos2d::Vec2 offset);
	// ���݂̃R���C�_�[�{�b�N�X�̎擾
	const std::vector <std::shared_ptr<ActionRect>>& GetCurrentCol(void)
	{
		return currentCol_;
	}
	// HP�̎擾
	const int& GetHp(void)
	{
		return hp_;
	}
	// �����̃^�C�v�擾
	virtual const ActorType& GetType(void)
	{
		return type_;
	}
	// �U���I�u�W�F�N�g�𔭎˂��Ă��邩�̎擾
	virtual bool GetFire(void)
	{
		return isFire_;
	}
	// �����Ă��邩�̃t���O�擾
	virtual bool AliveCheck(void)
	{
		return isAlive_;
	}
	// ���W���[���̓o�^
	virtual  void ActModuleRegistration(void) = 0;							

	// �U����`�̎擾
	const cocos2d::Rect& GetAttackCol(void)
	{
		return attackCol_;
	}
	// ��Ұ�ދ�`�̎擾
	const cocos2d::Rect& GetDamageCol(void)
	{
		return damageCol_;
	}
	// �e�R���C�_�[���̐ݒ�
	virtual void SetCollider(void);

	// �����蔻��
	virtual bool OnHit(const cocos2d::Rect& collision);
	virtual void SetIsHitAttack(bool flg);
private:
	std::vector<std::string> split(std::string str, std::string separator);	// xml�f�[�^���J���}�ŋ�؂�
protected:
	void XmlActDataRead(std::string string, OPRT_state* oprt);		// �A�N�V�����f�[�^�ǂݍ���
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
	// ���݂̃A�j���[�V����
	std::string currentAnimation_;
	// 1�t���[���O�̃A�j���[�V����
	std::string previousAnimation_;
	// �A�j���[�V�������Ă��鎞�͂��̃t���[���ϐ������Z
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
	// �����̃R���C�_�[�f�[�^
	std::unordered_map<std::string,
		std::vector<std::vector<std::shared_ptr<ActionRect>>>>collider_;
	// ���݂̃R���C�_�[
	std::vector <std::shared_ptr<ActionRect>> currentCol_;
	// �d��
	std::unique_ptr<Gravity> gravity_;
	// �_���[�W��H������t���O
	//bool onDamaged_;
	// �U�����Ă��邩�̃t���O
	bool isAttacking_;
	// �U���ɓ����������̔�������
	bool isHitAttack_;
	// �U����`
	AttackRect attackRect_;
	// �������g�����݂��郌�C���[
	cocos2d::Layer& myLayer_;

	cocos2d::Layer* attackLayer_;
	// �U���I�u�W�F�N�g���o���Ă��邩�̃t���O
	bool isFire_;
	// �����Ă��邩����ł��邩�̃t���O
	bool isAlive_;

	// ActionCtl���Ăяo��
	ActionCtl actCtl_;		

	//�ǂݍ��񂾃t�@�C���f�[�^�̕ۑ�
	DataTable fileLoad_;

	// �U����`�̊i�[��
	// first : ��`�f�[�^
	// second : �U����`�̖��O
	cocos2d::Rect attackCol_;
	// �_���[�W��`�̊i�[��
	cocos2d::Rect damageCol_;
};