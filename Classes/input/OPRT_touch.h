#pragma once
#include <array>
#include <vector>
#include <map>
#include "OPRT_state.h"

#define SWIPE_LEFT   1
#define SWIPE_RIGHT  3
#define SWIPE_DOWN   2
#define SWIPE_UP     4
#define SWIPE_CENTER 0

#define MULTI_TOUCH_MAX 2		// 同時に行える操作の数

// キー情報関係
struct KeyData {
	std::array<bool, static_cast<int>(BUTTON::MAX)> data_;					// keyflgで情報を入れる
	std::array<bool, static_cast<int>(BUTTON::MAX)> oldData_;
	std::array<bool, static_cast<int>(BUTTON::MAX)> input_;
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
	OPRT_touch(cocos2d::Sprite* sp);
	~OPRT_touch();

	OPRT_TYPE GetType(void);				// タッチ入力であることを返す
	void update();
	cocos2d::Point swipeStartPoint;			// スワイプ操作の最初の座標
	cocos2d::Point swipeEndPoint;			// スワイプ操作の終わりの座標
	int swipeRotate;						// スワイプの方向

	const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetNowData(void);	// 今の情報を返す
	const std::array<bool, static_cast<int>(BUTTON::MAX)> &GetOldData(void);	// 1フレーム前の情報を返す

	// マルチタップ(バーチャルパッド含む)
	void StartSwipe(cocos2d::Touch* touch);	// スワイプが始まった時
	void MoveSwipe(cocos2d::Touch* touch);	// スワイプの移動が行われた時
	void EndSwipe(cocos2d::Touch* touch);	// スワイプが終わった時

	void SetupTouch(cocos2d::Sprite* delta);	// タッチ操作のセットアップ
	std::vector<Touches> touchVectors;
	bool moveFlag;

	void ResetKey(void);						// マップ切替時のキーリセット
private:
	KeyData keyData_;							// キーのデータが格納される変数
	std::string trStr_;							// トランスフォーム切替で使用する変数
	std::map<std::string, BUTTON> map_;			// ボタン名をキーにしてボタンの押下/非押下処理
	std::map<BUTTON,std::vector<std::string>> sceneBtn_;	// Sceneに対するボタンの情報を保存する
	cocos2d::Node* SetBtnNameInfo(std::string str, BUTTON button);	// 現在の表示Sceneからボタン情報を一致させてreturnする
};