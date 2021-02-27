#pragma once

#include <array>
#include <map>
#include "OPRT_state.h"

#define SWIPE_LEFT 1
#define SWIPE_RIGHT 3
#define SWIPE_DOWN 2
#define SWIPE_UP 4
#define SWIPE_CENTER 0

// �L�[���֌W
struct KeyData {
	std::array<bool, static_cast<int>(BUTTON::MAX)> _data;					// keyflg�ŏ�������
	std::array<bool, static_cast<int>(BUTTON::MAX)> _oldData;
	std::array<bool, static_cast<int>(BUTTON::MAX)> _input;
};

struct Touches
{
	cocos2d::Vec2 pos;
	bool isMoveTouch;
	bool isAttackTouch;
	Touches() : pos(cocos2d::Vec2::ZERO), isMoveTouch(false), isAttackTouch(false) {};
};

struct OPRT_touch : OPRT_state
{
	OPRT_touch(cocos2d::Sprite* delta);
	~OPRT_touch();

	OPRT_TYPE GetType(void);				// �^�b�`���͂ł��邱�Ƃ�Ԃ�
	void update();
	cocos2d::Point touchStartPoint;			// �^�b�`�̍ŏ��̍��W
	cocos2d::Point touchEndPoint;			// �^�b�`�̏I���̍��W
	int swipeRotate;						// �X���C�v�̕���
	//void touchStart(cocos2d::Touch* touch);	// �X���C�v���n�܂�����
	//void touchMove(cocos2d::Touch* touch);	// �X���C�v�̈ړ����s��ꂽ��
	//void touchEnd(cocos2d::Touch* touch);	// �X���C�v���I�������

	const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetNowData(void);	// ���̏���Ԃ�
	const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetOldData(void);	// 1�t���[���O�̏���Ԃ�

	// �}���`�^�b�v�e�X�g(�o�[�`�����p�b�h�܂�)
	void touchesStart(cocos2d::Touch* touch);	// �^�b�`���n�܂�����
	void touchesMove(cocos2d::Touch* touch);	// �X���C�v�̈ړ����s��ꂽ��
	void touchesEnd(cocos2d::Touch* touch);	// �^�b�`(�X���C�v)���I�������

	void touchesflg(cocos2d::Sprite* delta);
	std::vector<Touches> touchVectors;
	bool moveFlag;

	void KeyReset(void);						// �}�b�v�ؑ֎��̃L�[���Z�b�g
private:
	KeyData _keyData;
	std::string trStr = "_toDark";
	std::map<std::string, BUTTON> map_;
	std::array<std::string, static_cast<int>(BUTTON::MAX)> nameTable_;
	BUTTON oldBtn_;
};