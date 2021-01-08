#pragma once
#include <array>
#include <memory>
#include "cocos2d.h"
#include "Direction/Direction.h"
#include "anim/AnimMng.h"
#include "anim/ActionCtl.h"
#include "input/OPRT_state.h"
#include "module/ActModule.h"
#include "obj/Actor.h"

// �r�b�g�t�B�[���h
struct Bits
{
	unsigned int FirstAttackFlg  : 1;		// 1���ڍU�����[�V�����t���O
	unsigned int SecondAttackFlg : 1;		// 2���ڍU�����[�V�����t���O
	unsigned int ThirdAttackFlg  : 1;		// 3���ڍU�����[�V�����t���O
	unsigned int TransfromFlg    : 1;		// �ϐg�p�t���O
};

#define AttackMove 60

class SkillBase;

class Player : public Actor
{
public:
	Player(int hp,cocos2d::Layer& myLayer, SkillBase* skillBasePtr);
	~Player();

	void Action(void) override;
	void ChangeDirection(void) override;
	void update(float delta) override;
	void AnimRegistrator(void) override;						// �A�j���[�V�����̓o�^
	const AttackRect& GetAttackRect(void);

	std::string GetAction(void);								// ���݂̃A�N�V���������擾����
	void SetAction(std::string action);							// ���݂̃A�N�V������Ԃ��Z�b�g����
	void SetDir(Direction dir);									// ���݂̕������Z�b�g����
	void KeyInputClear(void);									// �}�b�v�̐ؑ֎��ɃL�[���͂����Z�b�g������
	static Player* CreatePlayer(int hp,cocos2d::Layer& myLayer, SkillBase* skillBasePtr);
private:
	void attackMotion(float sp);								// �U�����[�V�����̐ݒ�
	void colliderVisible(void);									// ��`�̕`��֘A
	void attackCollider(std::string str, cocos2d::Node* col, float& pos);	// �U����`�̃I�t�Z�b�g
	bool gameOverAction(void);									// �Q�[���I�[�o�[���̃A�j���[�V��������(true:�Đ��I��,false:�Đ���)
	void actModuleRegistration(void);							// ���W���[���̓o�^

	OPRT_state* oprtState_;										// OPRT_state�̂��̂��Ăяo��

	std::string actionOld_;										// 1�t���[���O�̃A�N�V����
	ActionCtl actCtl_ = ActionCtl();							// ActionCtl���Ăяo��

	static int no_;												// �v���C���[�l���̔c������ϐ�
	int myNo_;													// �����̔ԍ���ێ�����ϐ�

	float oldPos_;												// �U�����[�V�������s�O�̍��W��ۊǂ���
	bool oldPosKeepFlg_;										// �U�����[�V�������s�O�̍��W��ۊǂ�1�x�����̎��s�ɂ��邽�߂̃t���O

	Bits bitFlg_;												// �r�b�g�t�B�[���h��p�����ϐ�
	float attackColOffset_;										// �U�����̃I�t�Z�b�g

	cocos2d::Action* gameOverAction_;							// �Q�[���I�[�o�[���ɍĐ�������̂̕ϐ�
	bool gameOverFlg_;											// gameOverAction��1�x�����Đ����邽�߂ɕK�v�ȕϐ�(true:�Đ�����or�Đ��I��,false:�Đ��O)
	bool deathFlg_;												// HP��0�ɂȂ����Ƃ��Ƀt���O�𗧂Ă�
	cocos2d::Vec2 deathPos_;									// ���S���̍��W��ێ����邽�߂̕ϐ�

	SkillBase* skillBase_;										// SkillBase�N���X�̃|�C���^���(�R���X�g���N�^�ōŏ��Ɏ擾)
	std::vector<std::string> plfile_;							// Player�̃X�L�����(.txt)������vector
};