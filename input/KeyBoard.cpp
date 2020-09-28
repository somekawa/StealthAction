#include <DxLib.h>
#include "KeyBoard.h"

CONTROLLER_TYPE KeyBoard::GetType(void)
{
	return CONTROLLER_TYPE::KEYBOARD;
}

bool KeyBoard::Setup(void)
{
	_keyData = { };

	_inputTbl = {
		//{InputID::UP   ,KEY_INPUT_UP}     ,
		{InputID::DOWN ,KEY_INPUT_DOWN}   ,
		{InputID::LEFT ,KEY_INPUT_LEFT}   ,
		{InputID::RIGHT,KEY_INPUT_RIGHT}  ,
		//{InputID::BTN1 ,KEY_INPUT_NUMPAD1},
		//{InputID::BTN2 ,KEY_INPUT_NUMPAD2}
	};
	return true;
}

void KeyBoard::Update(void)
{
	// すべてのキー状態を取得
	GetHitKeyStateAll(_keyData.data());
	// 該当場所のデータを格納
	// inputID分ループさせて格納させたい
	for (auto id : InputID())
	{
		_data[id][static_cast<int>(TRG::OLD)] = _data[id][static_cast<int>(TRG::NOW)];
		// テーブルにidで対応させてkeydataからとりだして_dataに格納する
		_data[id][static_cast<int>(TRG::NOW)] = _keyData[_inputTbl[id]];
	}
}

void KeyBoard::ChangeStage(int no, std::string str)
{
}

void KeyBoard::Rotate(int no)
{
	// 処理なし
}

// ここを継承してAIクラス
// _keyDataを作るようにしてあげればいい(updateをvirtual化)