#pragma once
#include <array>
#include <memory>
#include "cocos2d.h"
#include "anim/AnimMng.h"
#include "anim/ActionCtl.h"
#include "input/OPRT_state.h"
#include "module/ActModule.h"
#include "obj/Actor.h"
#include "obj/Player.h"

class Player : public Actor
{
public:
	Player();

	~Player();

	void Action(void);
	void ChangeDirection(void);

	void update(float sp);

	std::string GetAction(void);								// ���݂̃A�N�V���������擾����
	void SetAction(std::string action);							// ���݂̃A�N�V������Ԃ��Z�b�g����
	void SetDir(DIR dir);										// ���݂̕������Z�b�g����
	// �A�j���[�V�����̓o�^
	void AnimRegistrator(void);
	static Player* CreatePlayer(void);
private:
	//CREATE_FUNC(Player);

	void attackMotion(float sp);

	// �A�j���[�V�����֌W
	std::string actionNow_;
	std::string actionOld_;
	DIR _dir_Now;

	cocos2d::FlipX* _flip;									    // �摜���]����
	bool _flipFlag = false;										// ���]���邩�̃t���O

	bool _AnimOneStopFlag = false;								// �A�j���[�V������1��Ŏ~�߂�(jump�̎��Ƃ�)

	// OPRT�֌W
	OPRT_state* _oprtState;										// OPRT_state�̂��̂��Ăяo��
	std::array<bool, static_cast<int>(BUTTON::MAX)> _nowData;	// OPRT_key����擾�����L�[��������ϐ�
	TOUCH_TIMMING _keyTouch;									// �L�[�̓��͏�Ԃ��󂯎��e�X�g�ϐ�

	// �A�N�V�����֌W
	void actModuleRegistration(void);							// ���W���[���̓o�^
	ActionCtl _actCtl = ActionCtl();							// ActionCtl���Ăяo��

	// �v���C���[�֌W
	void CharCallData(cocos2d::Director* director, const char * charName);		// player�̏��
	void OutOfMapCheck(void);									// player��map�͈̔͊O�ɏo�Ă��Ȃ����̊m�F
	cocos2d::TMXLayer* _player;									// player�̃L�����f�[�^
	cocos2d::Vec2 _outOfMapCheckVel = { 0,0 };					// OutOfMapCheck�Ɏg��
	cocos2d::Vec2 _plPos;										// player��pos
	cocos2d::Vec2 _collisionBuckSpeed;							// �����蔻��Ŏg�������Ԃ��p�̒l
	cocos2d::Vec2 _gravity  = { 0.0f,0.0f };					// �d��
	cocos2d::Vec2 _velocity = { 0.0f,0.0f };					// ���x
	bool _jumpFlg = false;										// �W�����v���Ă��邩����

	// �}�b�v�֌W
	void CollisionMapCallData(cocos2d::Director* director, const char* mapName, const char* collisionName);	// �����蔻�肵����map�̏��
	cocos2d::TMXLayer* _CollisionData;							// �R���W�����}�b�v�̃f�[�^
	const int _tileChip = 48;									// 1�^�C��������̑傫��
	cocos2d::Size _ColLayerSize;								// �R���W�����}�b�v�̑傫��

	float cntTest = 0.0f;
	cocos2d::Vec2 _attackCheckL;
	cocos2d::Vec2 _attackCheckR;

	static int no_;
	int myNo_;

	bool attackflg = false;
	float oldPos_;
};