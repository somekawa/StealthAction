#pragma once

// �{�^���̎��
enum class InputID {
	//UP,
	DOWN,
	LEFT,
	RIGHT,
	//BTN1,
	//BTN2,
	MAX
};

static InputID begin(InputID) { return InputID::DOWN; };
static InputID end(InputID) { return InputID::MAX; };
//InputID operator++(InputID& state) { return state = static_cast<InputID>(static_cast<int>(state) + 1); };
static InputID operator++(InputID& state) { return (state = InputID(std::underlying_type<InputID>::type(state) + 1)); };
//underlying_type = ���̌^�𒲂ׂĂ���閽��
//enum class�͉��������Ȃ����int�^�ɂȂ邩�獡���int�����ɓ����Ă��邱�ƂɂȂ�

// �|�C���^�^�̂��̂��K�v�ɂȂ邩��
static InputID operator* (InputID state) { return (state); };

// ���̂�1�����ɂ��Ȃ��ƃG���[���ł�
// ���ꂼ��̏ꏊ�ō���邱�Ƃ��_����static�ŃA�h���X�����ʂɂ��悤