#pragma once
#include <array>
#include <memory>
#include "cocos2d.h"
#include "anim/AnimMng.h"
#include "anim/ActionCtl.h"
#include "input/OPRT_state.h"
#include "module/ActModule.h"
#include "obj/Actor.h"
#include "obj/Player.h"

class Player : public Actor
{
public:
	Player(std::unordered_map<std::string,
		std::vector<std::vector<std::shared_ptr<ActionRect>>>>&collider);

	//Player();
	~Player();

	void Action(void);
	void ChangeDirection(void);

	void update(float sp);

	std::string GetAction(void);								// 現在のアクション情報を取得する
	void SetAction(std::string action);							// 現在のアクション状態をセットする
	void SetDir(DIR dir);										// 現在の方向をセットする

	static Player* CreatePlayer(std::unordered_map<std::string,
		std::vector<std::vector<std::shared_ptr<ActionRect>>>>&collider);
	//static Player* createPlayer();
private:
	//CREATE_FUNC(Player);

	// アニメーション関係
	void Anim_Registration(cocos2d::Sprite* delta);				// アニメーションの登録
	std::string actionNow_;
	std::string actionOld_;
	DIR _dir_Now;

	cocos2d::FlipX* _flip;									    // 画像反転処理
	bool _flipFlag = false;										// 反転するかのフラグ

	bool _AnimOneStopFlag = false;								// アニメーションを1回で止める(jumpの時とか)

	// OPRT関係
	OPRT_state* _oprtState;										// OPRT_stateのものを呼び出す
	std::array<bool, static_cast<int>(BUTTON::MAX)> _nowData;	// OPRT_keyから取得したキー情報を入れる変数
	TOUCH_TIMMING _keyTouch;									// キーの入力状態を受け取るテスト変数

	// アクション関係
	void actModuleRegistration(void);							// モジュールの登録
	ActionCtl _actCtl = ActionCtl();							// ActionCtlを呼び出す

	// プレイヤー関係
	void CharCallData(cocos2d::Director* director, const char * charName);		// playerの情報
	void OutOfMapCheck(void);									// playerがmapの範囲外に出ていないかの確認
	cocos2d::TMXLayer* _player;									// playerのキャラデータ
	cocos2d::Vec2 _outOfMapCheckVel = { 0,0 };					// OutOfMapCheckに使う
	cocos2d::Vec2 _plPos;										// playerのpos
	cocos2d::Vec2 _collisionBuckSpeed;							// 当たり判定で使う押し返す用の値
	cocos2d::Vec2 _gravity  = { 0.0f,0.0f };					// 重力
	cocos2d::Vec2 _velocity = { 0.0f,0.0f };					// 速度
	bool _jumpFlg = false;										// ジャンプしているか判定

	// マップ関係
	void CollisionMapCallData(cocos2d::Director* director, const char* mapName, const char* collisionName);	// 当たり判定したいmapの情報
	cocos2d::TMXLayer* _CollisionData;							// コリジョンマップのデータ
	const int _tileChip = 48;									// 1タイル当たりの大きさ
	cocos2d::Size _ColLayerSize;								// コリジョンマップの大きさ

	float cntTest = 0.0f;
	cocos2d::Vec2 _attackCheckL;
	cocos2d::Vec2 _attackCheckR;

	static int no_;
	int myNo_;
};