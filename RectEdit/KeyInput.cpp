#include <DxLib.h>
#include "KeyInput.h"

KeyInput::KeyInput()
{
}

KeyInput::~KeyInput()
{
}

void KeyInput::UpDate(void)
{
	keyOld_ = keyNow_;

	GetHitKeyStateAll(&keyNow_[0]);
}

const KeyArray& KeyInput::GetInput(InputFlag flg)const
{
	return (flg == InputFlag::Now ? keyNow_ : keyOld_);
}
