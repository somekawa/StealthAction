#pragma once

#include <vector>
#include <array>
#include <list>
#include "OPRT_state.h"

// キー情報関係
struct KeyData {
	std::array<bool, static_cast<int>(BUTTON::MAX)> _data;					// keyflgで情報を入れる
	std::array<bool, static_cast<int>(BUTTON::MAX)> _oldData;
	std::array<bool, static_cast<int>(BUTTON::MAX)> _input;
};

struct OPRT_key : OPRT_state
{
	OPRT_key(cocos2d::Sprite* sprite);
	~OPRT_key();

	OPRT_TYPE GetType(void);					// キーボード入力であることを返す
	void update();								// アニメーション切替もここに入ってます
	void keyflg(cocos2d::Sprite* sprite);		// ボタンを押したとき・離したときの処理
	const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetNowData(void);	// dataの情報を返す(現在はplayerで使っている)
	const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetOldData(void);
private:
	KeyData _keyData;
};

