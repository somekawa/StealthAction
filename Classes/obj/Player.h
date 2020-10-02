#pragma once

#include <array>
#include "cocos2d.h"
#include "anim/AnimMng.h"
#include "anim/ActionCtl.h"
#include "input/OPRT_state.h"
#include "module/ActModule.h"
#include "obj/Player.h"

class Player : public cocos2d::Sprite
{
public:
	static Player* createPlayer();

	Player();
	~Player();

	void update(float sp);
	//static Player* createPL();								// �}�N���̒�����

	ACTION GetAction(void);										// ���݂̃A�N�V���������擾����
	void SetAction(ACTION action);								// ���݂̃A�N�V������Ԃ��Z�b�g����

private:
	CREATE_FUNC(Player);

	// �A�j���[�V�����֌W
	void Anim_Registration(cocos2d::Sprite* delta);				// �A�j���[�V�����̓o�^
	void AnimCheck(cocos2d::Sprite* delta);						// �A�j���[�V�����؂�ւ��w��
	const char* _animTable[static_cast<int>(ACTION::MAX)] = { "idle","idle","jump","jump","duck","run","duck","duck","attack" }; // AnimCheck�Ŏg��

	ACTION _action_Now;											// ���݂̃A�N�V�������
	ACTION _action_Old;											// 1�t���[���O�̃A�N�V�������

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
};