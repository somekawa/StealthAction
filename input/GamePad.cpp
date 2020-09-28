#include <DxLib.h>
#include "GamePad.h"

CONTROLLER_TYPE GamePad::GetType(void)
{
	return CONTROLLER_TYPE::GAMEPAD;
}

bool GamePad::Setup(void)
{
	// 1つつないだとき
	padVec.emplace_back(DX_INPUT_PAD1);

	_inputTbl = {
		//{InputID::UP   ,PAD_INPUT_UP}   ,
		{InputID::DOWN ,PAD_INPUT_DOWN} ,
		{InputID::LEFT ,PAD_INPUT_LEFT} ,
		{InputID::RIGHT,PAD_INPUT_RIGHT},
		//{InputID::BTN1 ,PAD_INPUT_1}    ,
		//{InputID::BTN2 ,PAD_INPUT_2}
	};
	return true;
}

void GamePad::Update(void)
{
	for (auto vec : padVec)
	{
		padInpVec.emplace_back(GetJoypadInputState(vec));
	}

	for (auto inpVec : padInpVec)
	{
		for (auto id : InputID())
		{
			_data[id][static_cast<int>(TRG::OLD)] = _data[id][static_cast<int>(TRG::NOW)];
			_data[id][static_cast<int>(TRG::NOW)] = ((inpVec & _inputTbl[id]) != 0);
		}
	}
}

void GamePad::ChangeStage(int no, std::string str)
{
	// つないでるコントローラーが1つのときだけ、操作ステージを変更できる
	if (GetJoypadNum() == 1)
	{
		if (str == "up")
		{
			if (no == 0)
			{
				padVec[0] = DX_INPUT_KEY_PAD1;
			}
			else
			{
				padVec[0] = 0;
			}
		}

		if (str == "down")
		{
			if (no == 1)
			{
				padVec[0] = DX_INPUT_KEY_PAD1;
			}
			else
			{
				padVec[0] = 0;
			}
		}
	}
}

void GamePad::Rotate(int no)
{
	// 処理なし
}