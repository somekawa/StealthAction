#pragma once
#include <memory>
#include <list>
#include <array>
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

// ���E�͈�
// �G�̎�ނɂ���Ď��E�͈͂����ꂼ�ꎝ���������̂�
struct VisionRange
{
	// �U���͈�
	float attack_;
	// �ǐՃ��[�h�ɂȂ�͈�
	float chase_;
	VisionRange():attack_(0.0f),chase_(0.0f) {};
	VisionRange(float attack, float chase) :attack_(attack), chase_(chase) {};
};

class Enemy :
	public Actor
{
public:
	Enemy(cocos2d::Vec2 pos,Player& player,
		BehaviorTree* aiTree,VisionRange visionRange,int hp,cocos2d::Layer& myLayer);
	~Enemy();

	virtual void Action(void) = 0;

	// ���E�ɓ��������̌v�Z�����Avision_��Ԃ�
	virtual const float& GetVision(void);

	// �������g�̖��O�̎擾
	const std::string& GetMyName(void)
	{
		return myName_;
	}

	// �e���E�͈͂̎擾
	virtual const VisionRange& GetVisionRange(void)
	{
		return visionRange_;
	}
	// �����݂��۱�ɑ��݂��鎩�����g������(�۱�ύX�̍ۂɎg�p)
	virtual void DeleteSelfOnFloor(void);

	virtual void AddAttackObj(const float& angle) = 0;

	virtual void ChangeDirection(void);
	virtual const float& DistanceCalcurator(void);
	// �����Ă��邩���׸ނ̾��
	void SetAlive(bool flg);
	// �����̍s���^�C�v�̎擾
	virtual const MoveType& GetMoveType(void)
	{
		return mType_;
	}
	// ���g�̌ŗLID�̾��
	void SetID(int id);
	// ���g�̌ŗLID�̎擾
	const int& GetID(void)
	{
		return id_;
	}

	virtual void update(float delta) = 0;
	// �����̍U�����v���C���[�ɓ������Ă��邩�̔���
	virtual bool OnAttacked(void);
	// ��ڲ԰�̓������`�Ƃ̓����蔻��
	virtual void CheckHitPLAttack(void);
	// �A�j���[�V�����̍X�V
	virtual void UpdateAnimation(float delta);
	// �A�j���[�V�����̕ύX
	// param@ animName: �ύX��A�j���[�V������
	virtual void ChangeAnimation(std::string animName);

	virtual void AnimRegistrator(void) = 0;
	// ���W���[���̓o�^
	virtual void actModuleRegistration(void) = 0;							

	// AI�s����Node��T�����Ď��s
	virtual void AIRun(void);
	// �ړ��^�C�v�̃Z�b�g
	// ��L��enum����
	// �s���^�C�v
	//enum class MoveType
	//{
	//	// ���������Ă��Ȃ�
	//	Non,
	//	// �ǐ�
	//	Chase,
	//	// ����
	//	Patrol,
	//	Max
	//};
	virtual void SetMoveType(MoveType type);

	// �ǂ⏰�ɂԂ����Ă��邩�̔���
	virtual bool CheckObjHit(void);
	// Map�̃I�u�W�F�N�g�ɓ����������̊֐�
	//virtual void CheckMapObjHit(float delta);
	// �����U��
	virtual void NormalAttack(void) = 0;
	// �X�L���g�p
	virtual void UseSkill(void) = 0;
	// Run or walk
	virtual void Patrol(void) = 0;
	virtual void Chase(void) = 0;
	// �W�����v
	virtual void Jump(void) = 0;
	// �W�����v����̗����郂�[�V����
	virtual void Fall(void) = 0;
	// �G�̏�Ԃ�\��--------------------
	// �ҋ@���[�V����
	virtual void Idle(void);
	// �v���C���[����̍U���̂��炢
	virtual void Hit(void);
	// ����
	virtual void Death(void);
	// ----------------------------------
private:

protected:

	// ��Ԃ̑J�ڗp�֐��߲��
	void(Enemy::* stateTransitioner_)(void);
	Player& player_;
	// �����̍U���������������̃t���O
	bool hittingToPlayer_;

	// ���E�͈�
	float vision_;
	cocos2d::Vec2 oldPos_;
	// �e�s�������{����ۂ̎��E�͈�
	VisionRange visionRange_;
	// �v���C���[�����F���Ă��邩�̃t���O
	bool identifiedPlayer_;
	// �r�w�C�r�A�c���[
	BehaviorTree* aiTree_;	
	// �r�w�C�r�A�f�[�^
	BehaviorData* behaviorData_;	
	// ���s���m�[�h
	NodeBase* activeNode_;			

	// �����ύX�t���O
	bool isChangeDir_;

	// �A�j���[�V�������ύX���ꂽ���̃t���O
	bool isChangedAnim_;

	// �ړ��̎��(patrol or chase)
	MoveType mType_;

	cocos2d::DrawNode* debugCircle_;
	// ��苗����������true�ɂȂ�
	bool isMoveComplete_;
	// ���g�̖��O
	std::string myName_;
	// ���g�̌ŗLID
	int id_;
};