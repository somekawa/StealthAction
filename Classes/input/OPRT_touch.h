#pragma once

#include <array>
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

struct OPRT_touch : OPRT_state
{
	OPRT_touch(cocos2d::Sprite* delta);
	~OPRT_touch();

	OPRT_TYPE GetType(void);				// �^�b�`���͂ł��邱�Ƃ�Ԃ�
	void update();
	cocos2d::Point touchStartPoint;			// �^�b�`�̍ŏ��̍��W
	cocos2d::Point touchEndPoint;			// �^�b�`�̏I���̍��W
	int swipeRotate;						// �X���C�v�̕���
	void touchStart(cocos2d::Touch* touch);	// �X���C�v���n�܂�����
	void touchMove(cocos2d::Touch* touch);	// �X���C�v�̈ړ����s��ꂽ��
	void touchEnd(cocos2d::Touch* touch);	// �X���C�v���I�������

	void touchflg(cocos2d::Sprite* delta);

	const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetNowData(void);	// ���̏���Ԃ�
	const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetOldData(void);	// 1�t���[���O�̏���Ԃ�

private:
	KeyData _keyData;
};


