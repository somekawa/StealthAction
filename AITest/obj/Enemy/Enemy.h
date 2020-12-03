#pragma once
#include "obj/Actor.h"

class Player;
class BehaviorTree;
class BehaviorData;
class NodeBase;

// �s���^�C�v
enum class MoveType
{
	// ���������Ă��Ȃ�
	Non,
	// �ǐ�
	Chase,
	// ����
	Patrol,
	Max
};

class Enemy :
	public Actor
{
public:
	Enemy(cocos2d::Vector<cocos2d::Node*>& player_, BehaviorTree* aiTree);
	~Enemy();

	virtual void Action(void) = 0;
	virtual void ChangeDirection(void);
	virtual const float& DistanceCalcurator(void);

	void SetIsAttacking(bool flg);

	// �U�����Ă��邩�̃t���O�擾�֐�
	virtual const bool& IsAttacking(void)
	{
		return isAttacking_;
	}
	// �����̍s���^�C�v�̎擾
	virtual const MoveType& GetMoveType(void)
	{
		return mType_;
	}

	// �A�j���[�V�����̍X�V
	virtual void UpdateAnimation(float delta);
	// �A�j���[�V�����̕ύX
	// param@ animName: �ύX��A�j���[�V������
	virtual void ChangeAnimation(std::string animName);

	virtual void AnimRegistrator(void) = 0;
	// AI�s����Node��T�����Ď��s
	virtual void AIRun(void);
	// �s���^�C�v�̃Z�b�g
	virtual void SetMoveType(MoveType type);
	
	// �����U��
	virtual void NormalAttack(void) = 0;
	// �X�L���g�p
	virtual void UseSkill(void) = 0;
	// �ҋ@���[�V����
	virtual void Idle(void) = 0;
	// Run or walk
	virtual void Patrol(void) = 0;
	virtual void Chase(void) = 0;
	// �W�����v
	virtual void Jump(void) = 0;
	// �W�����v����̗����郂�[�V����
	virtual void Fall(void) = 0;
	// �v���C���[����̍U���̂��炢
	virtual void Hit(void) = 0;
	// ����
	virtual void Death(void) = 0;
private:

protected:
	cocos2d::Vector<cocos2d::Node*>& player_;

	BehaviorTree* aiTree_;			// �r�w�C�r�A�c���[
	BehaviorData* behaviorData_;	// �r�w�C�r�A�f�[�^
	NodeBase* activeNode_;			// ���s���m�[�h

	// �U�����Ă��邩�̃t���O
	bool isAttacking_;
	// �A�j���[�V�������ύX���ꂽ���̃t���O
	bool isChangedAnim_;

	MoveType mType_;
};