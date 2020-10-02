#pragma once

#include <vector>
#include <array>
#include <list>
#include "OPRT_state.h"

// �L�[���֌W
struct KeyData {
	std::array<bool, static_cast<int>(BUTTON::MAX)> _data;					// keyflg�ŏ�������
	std::array<bool, static_cast<int>(BUTTON::MAX)> _oldData;
	std::array<bool, static_cast<int>(BUTTON::MAX)> _input;
};

struct OPRT_key : OPRT_state
{
	OPRT_key(cocos2d::Sprite* sprite);
	~OPRT_key();

	OPRT_TYPE GetType(void);					// �L�[�{�[�h���͂ł��邱�Ƃ�Ԃ�
	void update();								// �A�j���[�V�����ؑւ������ɓ����Ă܂�
	void keyflg(cocos2d::Sprite* sprite);		// �{�^�����������Ƃ��E�������Ƃ��̏���
	const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetNowData(void);	// data�̏���Ԃ�(���݂�player�Ŏg���Ă���)
	const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetOldData(void);
private:
	KeyData _keyData;
};

