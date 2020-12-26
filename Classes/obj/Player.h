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
//#include "obj/Player.h"

// �r�b�g�t�B�[���h
struct Bits
{
	unsigned int FirstAttackFlg  : 1;		// 1���ڍU�����[�V�����t���O
	unsigned int SecondAttackFlg : 1;		// 2���ڍU�����[�V�����t���O
	unsigned int ThirdAttackFlg  : 1;		// 3���ڍU�����[�V�����t���O
	unsigned int TransfromFlg    : 1;		// �ϐg�p�t���O
};

#define AttackMove 60

class Player : public Actor
{
public:
	Player(int hp,cocos2d::Layer& myLayer);

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
	static Player* CreatePlayer(int hp,cocos2d::Layer& myLayer);

private:
	//CREATE_FUNC(Player);

	void attackMotion(float sp);								// �U�����[�V�����̐ݒ�
	void colliderVisible(void);									// ��`�̕`��֘A
	void attackCollider(std::string str, cocos2d::Node* col, float& pos);	// �U����`�̃I�t�Z�b�g

	// �A�j���[�V�����֌W
	//std::string actionNow_;
	std::string actionOld_;

	// OPRT�֌W
	OPRT_state* _oprtState;										// OPRT_state�̂��̂��Ăяo��
	std::array<bool, static_cast<int>(BUTTON::MAX)> _nowData;	// OPRT_key����擾�����L�[��������ϐ�
	TOUCH_TIMMING _keyTouch;									// �L�[�̓��͏�Ԃ��󂯎��e�X�g�ϐ�

	// �A�N�V�����֌W
	void actModuleRegistration(void);							// ���W���[���̓o�^
	ActionCtl _actCtl = ActionCtl();							// ActionCtl���Ăяo��

	static int no_;
	int myNo_;

	float oldPos_;												// �U�����[�V�������s�O�̍��W��ۊǂ���
	bool oldPosKeepFlg_;										// �U�����[�V�������s�O�̍��W��ۊǂ�1�x�����̎��s�ɂ��邽�߂̃t���O

	Bits bitFlg_;

	// �U�����̃I�t�Z�b�g�e�X�g
	float attackColOffset_;
};

// �܂��͓G�Ƃ̍U����`�ƃ_���[�W��`�̓����蔻��
// �G�ɍU���ł����Ƃ��ɁA�ǂ̓G�ɂǂꂾ���_���[�W������������`����p�̊֐�������