#pragma once
#include <array>

#include "InputFlag.h"

// ���ް�ނ�����
using KeyArray = std::array<char, 256>;

class KeyInput
{
public:
	KeyInput();
	~KeyInput();

	// �X�V
	void UpDate(void);
	// ���͏��̎擾
	const KeyArray& GetInput(InputFlag flg)const;

private:
	KeyArray keyNow_;
	KeyArray keyOld_;
};

