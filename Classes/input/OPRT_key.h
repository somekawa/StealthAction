#pragma once

#include <vector>
#include <map>
#include <array>
#include <list>
#include <utility>
#include "OPRT_state.h"

// キー情報関係
struct KeyData {
	std::array<bool, static_cast<int>(BUTTON::MAX)> data_;				// Keyflgで情報を入れる
	std::array<bool, static_cast<int>(BUTTON::MAX)> oldData_;
	std::array<bool, static_cast<int>(BUTTON::MAX)> input_;
};

struct OPRT_key : OPRT_state
{
	OPRT_key(cocos2d::Sprite* sprite);
	~OPRT_key();

	OPRT_TYPE GetType(void);					// キーボード入力であることを返す
	void update();								// アニメーション切替もここに入ってます
	void SetUpKey(cocos2d::Sprite* sprite);		// ボタンを押したとき・離したときの処理
	const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetNowData(void);	// dataの情報を返す(現在はplayerで使っている)
	const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetOldData(void);
	void ResetKey(void);						// マップ切替時のキーリセット
private:
	KeyData keyData_;							// キーのデータが格納される変数
	std::map<cocos2d::EventKeyboard::KeyCode, BUTTON> keyMap_;	// first:入力キー second:実行アクション
};