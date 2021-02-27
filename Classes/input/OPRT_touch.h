#pragma once

#include <array>
#include <map>
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

struct Touches
{
	cocos2d::Vec2 pos;
	bool isMoveTouch;
	bool isAttackTouch;
	Touches() : pos(cocos2d::Vec2::ZERO), isMoveTouch(false), isAttackTouch(false) {};
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
	//void touchStart(cocos2d::Touch* touch);	// スワイプが始まった時
	//void touchMove(cocos2d::Touch* touch);	// スワイプの移動が行われた時
	//void touchEnd(cocos2d::Touch* touch);	// スワイプが終わった時

	const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetNowData(void);	// 今の情報を返す
	const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetOldData(void);	// 1フレーム前の情報を返す

	// マルチタップテスト(バーチャルパッド含む)
	void touchesStart(cocos2d::Touch* touch);	// タッチが始まった時
	void touchesMove(cocos2d::Touch* touch);	// スワイプの移動が行われた時
	void touchesEnd(cocos2d::Touch* touch);	// タッチ(スワイプ)が終わった時

	void touchesflg(cocos2d::Sprite* delta);
	std::vector<Touches> touchVectors;
	bool moveFlag;

	void KeyReset(void);						// マップ切替時のキーリセット
private:
	KeyData _keyData;
	std::string trStr = "_toDark";
	std::map<std::string, BUTTON> map_;
	std::array<std::string, static_cast<int>(BUTTON::MAX)> nameTable_;
	BUTTON oldBtn_;
};