#pragma once
#include <array>
#include <memory>
#include "cocos2d.h"
#include "Direction/Direction.h"
#include "anim/AnimMng.h"

#include "obj/Actor.h"

// ビットフィールド
struct Bits
{
	bool FirstAttackFlg  : 1;		// 1撃目攻撃モーションフラグ
	bool SecondAttackFlg : 1;		// 2撃目攻撃モーションフラグ
	bool ThirdAttackFlg  : 1;		// 3撃目攻撃モーションフラグ
	bool TransfromFlg    : 1;		// 変身用フラグ
	bool DashFlg		 : 1;
};

#define AttackMove 30
#define AttackNumDef 10				// 通常状態の攻撃力

class SkillBase;
class ResidualShadow;
class Player : public Actor
{
public:
	Player(int hp,cocos2d::Layer& myLayer, cocos2d::Layer& enemyLayer, SkillBase* skillBasePtr);
	~Player();

	void Action(void) override;
	void ChangeDirection(float delta) override;
	void update(float delta) override;
	void AnimRegistrator(void) override;						// アニメーションの登録
	const AttackRect& GetAttackRect(void);

	std::string GetAction(void);								// 現在のアクション情報を取得する
	void SetAction(std::string action);							// 現在のアクション状態をセットする
	void SetDir(Direction dir);									// 現在の方向をセットする
	void KeyInputClear(void);									// マップの切替時にキー入力をリセットしたい
	static Player* CreatePlayer(int hp,cocos2d::Layer& myLayer, cocos2d::Layer& enemyLayer, SkillBase* skillBasePtr);
	bool GetGameOverFlg(void);									// ゲームオーバーシーンへ遷移してもよいか判断する
	int GetGiveDamageNum(void);									// 敵に与えるダメージ量(lightとdarkでは値が変わる)
	std::string GetPlayerColor(void);							// 現在のプレイヤーカラー
private:
	void attackMotion(float delta);								// 攻撃モーションの設定
	void transformMotion(float delta);							// トランスフォームの設定
	void dashMotion(float delta);								// ダッシュ(回避)の設定
	void colliderVisible(void);									// 矩形の描画関連
	void attackCollider(std::string str, cocos2d::Node* col, float& pos);	// 攻撃矩形のオフセット
	bool gameOverAction(void);									// ゲームオーバー時のアニメーション制御(true:再生終了,false:再生中)
	void actModuleRegistration(void);
	OPRT_state* oprtState_;										// OPRT_stateのものを呼び出す

	std::string actionOld_;										// 1フレーム前のアクション

	static int no_;												// プレイヤー人数の把握する変数
	int myNo_;													// 自分の番号を保持する変数

	//float oldPos_;											// 攻撃モーション実行前の座標を保管する
	bool oldPosKeepFlg_;										// 攻撃モーション実行前の座標を保管を1度だけの実行にするためのフラグ

	Bits bitFlg_;												// ビットフィールドを用いた変数
	float attackColOffset_;										// 攻撃時のオフセット

	cocos2d::Action* gameOverAction_;							// ゲームオーバー時に再生するものの変数
	bool gameOverFlg_;											// gameOverActionを1度だけ再生するために必要な変数(true:再生直後or再生終了,false:再生前)
	bool deathFlg_;												// HPが0になったときにフラグを立てる
	cocos2d::Vec2 deathPos_;									// 死亡時の座標を保持するための変数

	void skillAction(void);
	SkillBase* skillBase_;										// SkillBaseクラスのポインタ情報(コンストラクタで最初に取得)
	SkillBase* skillSprite = nullptr;							// skillSpriteの情報(Skill発動時に更新)
	std::vector<std::string> plfile_;							// Playerのスキル情報(.txt)を入れるvector
	cocos2d::Layer& enemyList_;
	std::string playerColor = "player_Light_";

	// 分身
	std::shared_ptr<ResidualShadow> resShadow_;
	// ダッシュテストで使ってた変数
	//float testnum = -4.0f;
	//float testnum = 0.00f;

	bool gameOverSceneFlg_ = false;
};