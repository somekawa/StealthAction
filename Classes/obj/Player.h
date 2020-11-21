#pragma once
#include <array>
#include <memory>
#include "cocos2d.h"
#include "Direction/Direction.h"
#include "anim/AnimMng.h"
#include "anim/ActionCtl.h"
#include "input/OPRT_state.h"
#include "module/ActModule.h"
#include "obj/Actor.h"
#include "obj/Player.h"

#define AttackMove 60

class Player : public Actor
{
public:
	Player();

	~Player();

	void Action(void) override;
	void ChangeDirection(void) override;
	void update(float sp) override;
	void AnimRegistrator(void) override;						// アニメーションの登録

	std::string GetAction(void);								// 現在のアクション情報を取得する
	void SetAction(std::string action);							// 現在のアクション状態をセットする
	void SetDir(Direction dir);									// 現在の方向をセットする
	static Player* CreatePlayer(void);
private:
	//CREATE_FUNC(Player);

	void attackMotion(float sp);								// 攻撃モーションの設定

	// アニメーション関係
	std::string actionNow_;
	std::string actionOld_;

	// OPRT関係
	OPRT_state* _oprtState;										// OPRT_stateのものを呼び出す
	std::array<bool, static_cast<int>(BUTTON::MAX)> _nowData;	// OPRT_keyから取得したキー情報を入れる変数
	TOUCH_TIMMING _keyTouch;									// キーの入力状態を受け取るテスト変数

	// アクション関係
	void actModuleRegistration(void);							// モジュールの登録
	ActionCtl _actCtl = ActionCtl();							// ActionCtlを呼び出す

	static int no_;
	int myNo_;

	float oldPos_;												// 攻撃モーション実行前の座標を保管する
	bool oldPosOnceKeepFlg_;									// 攻撃モーション実行前の座標を保管を1度だけの実行にするためのフラグ
	bool SecondAttackFlg_;										// 2撃目攻撃モーションフラグ
};