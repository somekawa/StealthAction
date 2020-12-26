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
//#include "obj/Player.h"

// ビットフィールド
struct Bits
{
	unsigned int FirstAttackFlg  : 1;		// 1撃目攻撃モーションフラグ
	unsigned int SecondAttackFlg : 1;		// 2撃目攻撃モーションフラグ
	unsigned int ThirdAttackFlg  : 1;		// 3撃目攻撃モーションフラグ
	unsigned int TransfromFlg    : 1;		// 変身用フラグ
};

#define AttackMove 60

class Player : public Actor
{
public:
	Player(int hp,cocos2d::Layer& myLayer);

	~Player();

	void Action(void) override;
	void ChangeDirection(void) override;
	void update(float delta) override;
	void AnimRegistrator(void) override;						// アニメーションの登録
	const AttackRect& GetAttackRect(void);

	std::string GetAction(void);								// 現在のアクション情報を取得する
	void SetAction(std::string action);							// 現在のアクション状態をセットする
	void SetDir(Direction dir);									// 現在の方向をセットする
	void KeyInputClear(void);									// マップの切替時にキー入力をリセットしたい
	static Player* CreatePlayer(int hp,cocos2d::Layer& myLayer);

private:
	//CREATE_FUNC(Player);

	void attackMotion(float sp);								// 攻撃モーションの設定
	void colliderVisible(void);									// 矩形の描画関連
	void attackCollider(std::string str, cocos2d::Node* col, float& pos);	// 攻撃矩形のオフセット

	// アニメーション関係
	//std::string actionNow_;
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
	bool oldPosKeepFlg_;										// 攻撃モーション実行前の座標を保管を1度だけの実行にするためのフラグ

	Bits bitFlg_;

	// 攻撃時のオフセットテスト
	float attackColOffset_;
};

// まずは敵との攻撃矩形とダメージ矩形の当たり判定
// 敵に攻撃できたときに、どの敵にどれだけダメージが入ったかを伝える用の関数をつくる