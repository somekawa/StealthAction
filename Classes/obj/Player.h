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
#include "obj/Player.h"

#define AttackMove 60

class Player : public Actor
{
public:
	Player();

	~Player();

	void Action(void) override;
	void ChangeDirection(void) override;
	void update(float sp) override;
	void AnimRegistrator(void) override;						// �A�j���[�V�����̓o�^

	std::string GetAction(void);								// ���݂̃A�N�V���������擾����
	void SetAction(std::string action);							// ���݂̃A�N�V������Ԃ��Z�b�g����
	void SetDir(Direction dir);									// ���݂̕������Z�b�g����
	static Player* CreatePlayer(void);
private:
	//CREATE_FUNC(Player);

	void attackMotion(float sp);								// �U�����[�V�����̐ݒ�

	// �A�j���[�V�����֌W
	std::string actionNow_;
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
	bool oldPosOnceKeepFlg_;									// �U�����[�V�������s�O�̍��W��ۊǂ�1�x�����̎��s�ɂ��邽�߂̃t���O
	bool SecondAttackFlg_;										// 2���ڍU�����[�V�����t���O
};