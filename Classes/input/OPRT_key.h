#pragma once

#include <vector>
#include <map>
#include <array>
#include <list>
#include <utility>
#include "OPRT_state.h"

// �L�[���֌W
struct KeyData {
	std::array<bool, static_cast<int>(BUTTON::MAX)> data_;				// Keyflg�ŏ�������
	std::array<bool, static_cast<int>(BUTTON::MAX)> oldData_;
	std::array<bool, static_cast<int>(BUTTON::MAX)> input_;
};

struct OPRT_key : OPRT_state
{
	OPRT_key(cocos2d::Sprite* sprite);
	~OPRT_key();

	OPRT_TYPE GetType(void);					// �L�[�{�[�h���͂ł��邱�Ƃ�Ԃ�
	void update();								// �A�j���[�V�����ؑւ������ɓ����Ă܂�
	void SetUpKey(cocos2d::Sprite* sprite);		// �{�^�����������Ƃ��E�������Ƃ��̏���
	const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetNowData(void);	// data�̏���Ԃ�(���݂�player�Ŏg���Ă���)
	const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetOldData(void);
	void ResetKey(void);						// �}�b�v�ؑ֎��̃L�[���Z�b�g
private:
	KeyData keyData_;							// �L�[�̃f�[�^���i�[�����ϐ�
	std::map<cocos2d::EventKeyboard::KeyCode, BUTTON> keyMap_;	// first:���̓L�[ second:���s�A�N�V����
};