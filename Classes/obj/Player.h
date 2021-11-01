// 担当場所
#pragma once
#include <cocos2d.h>
#include <vector>
#include <array>
#include <memory>
#include <utility>
#include "Direction/Direction.h"
#include "obj/Actor.h"

#define ATTAKFLAG 1    //00000001

// パラメータ
struct PlParam
{
	std::string modeName;		// モードの名前
	float runSpeedUp;			// 移動速度への加算値
	float attackDef;			// 通常の攻撃力
	int attackChain;			// 最大連撃数
	std::vector<std::pair<std::string,float>> chainAnimName;	// first：攻撃アニメーション名,second：ダメージ追加倍率
};

// ビットフィールド
struct Bits
{
	bool transformFlg    : 1;		// 変身用フラグ
	bool dashFlg		 : 1;		// ダッシュアクションフラグ
	bool gameOverFlg     : 1;		// GameOverActionを1度だけ再生するために必要な変数(true:再生直後or再生終了,false:再生前)
	bool gameOverSceneFlg: 1;		// GameSceneにゲームオーバーへの遷移を知らせる為のフラグ
	bool deathFlg		 : 1;		// HPが0になったときにフラグを立てる
	bool oldPosKeepFlg_  : 1;		// 攻撃モーション実行前の座標を保存するフラグ
	bool reserve		 : 2;		// リザーブ領域
};

#define DashMove 30.0f										// ダッシュアクションの移動量
#define Calculation(X) ((X * 100.0f) - 16.0f) / 4.0f		// ダッシュアクションの指数関数計算時に利用
#define AttackSizeCorrection	15.0f						// 攻撃画像のサイズ差による補正
#define TransformSizeCorrection 10.0f						// モード変更画像のサイズ差による補正
#define ReceiveDamage			10.0f						// 敵から受けるダメージ量
#define SlipDamage				0.05f						// スリップダメージ量

class SkillBase;
class ResidualShadow;

class Player : public Actor
{
public:
	Player(int hp,cocos2d::Layer& myLayer, cocos2d::Layer& enemyLayer, SkillBase* skillBasePtr);
	~Player();

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
	float GetRunSpeedUp(void);									// 現在のモードから移動加算値を取得する

private:
	void AttackMotion(float delta);								// 攻撃モーションの設定
	void TransformMotion(float delta);							// トランスフォームの設定
	void DashMotion(float delta);								// ダッシュ(回避)の設定
	bool GameOverAction(void);									// ゲームオーバー時のアニメーション制御(true:再生終了,false:再生中)
	void ActModuleRegistration(void);							// アクションモジュールの登録
	void SkillAction(void);										// スキル処理

	OPRT_state* oprtState_;										// OPRT_stateのものを呼び出す
	std::string actionOld_;										// 1フレーム前のアクション
	std::string playerMode_;									// Light/Darkモードの切替用変数
	static int no_;												// プレイヤー人数の把握する変数
	const int myNo_;											// 自分の番号を保持する変数
	Bits bitFlg_;												// ビットフィールドを用いた変数

	cocos2d::Action* gameOverAction_;							// ゲームオーバー時に再生するものの変数
	cocos2d::Vec2 deathPos_;									// 死亡時の座標を保持するための変数

	SkillBase* skillBase_;										// SkillBaseクラスのポインタ情報(コンストラクタで最初に取得)
	SkillBase* skillSprite_;									// skillSprite_の情報(Skill発動時に更新)
	cocos2d::Layer& enemyList_;									// スキル発動相手を取得する際に利用

	std::shared_ptr<ResidualShadow> resShadow_;					// 分身

	std::vector<std::string> SkillTbl_;							// スキル名のテーブル
	std::vector<std::string> SkillSETbl_;						// スキルSE名のテーブル

	char attackMotion_;													// 攻撃モーションフラグ(1bitずつ使用)
	std::map<std::string, std::vector<std::string>> attackMotionName_;	// キー:プレイヤーモード,データ:攻撃モーション名
	const cocos2d::Vec2 charSize_;								// キャラ画像サイズ

	std::vector<PlParam> plParam_;								// モード毎のパラメータ保存変数
};