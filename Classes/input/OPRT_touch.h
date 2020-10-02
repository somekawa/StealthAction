#pragma once

#include <array>
#include "OPRT_state.h"

#define SWIPE_LEFT 1
#define SWIPE_RIGHT 3
#define SWIPE_DOWN 2
#define SWIPE_UP 4
#define SWIPE_CENTER 0

// キー情報関係
struct KeyData {
	std::array<bool, static_cast<int>(BUTTON::MAX)> _data;					// keyflgで情報を入れる
	std::array<bool, static_cast<int>(BUTTON::MAX)> _oldData;
	std::array<bool, static_cast<int>(BUTTON::MAX)> _input;
};

struct OPRT_touch : OPRT_state
{
	OPRT_touch(cocos2d::Sprite* delta);
	~OPRT_touch();

	OPRT_TYPE GetType(void);				// タッチ入力であることを返す
	void update();
	cocos2d::Point touchStartPoint;			// タッチの最初の座標
	cocos2d::Point touchEndPoint;			// タッチの終わりの座標
	int swipeRotate;						// スワイプの方向
	void touchStart(cocos2d::Touch* touch);	// スワイプが始まった時
	void touchMove(cocos2d::Touch* touch);	// スワイプの移動が行われた時
	void touchEnd(cocos2d::Touch* touch);	// スワイプが終わった時

	void touchflg(cocos2d::Sprite* delta);

	const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetNowData(void);	// 今の情報を返す
	const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetOldData(void);	// 1フレーム前の情報を返す

private:
	KeyData _keyData;
};


