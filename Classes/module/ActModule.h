// 全て担当
#pragma once
#include <array>
#include <list>
#include <map>
#include <string>
#include "input/OPRT_state.h"
#include "../AnimationEnum/AnimationType.h"

struct ActModule;
class Enemy;

// 関数オブジェクトをlistに登録する
using funcActModule = std::function<bool(cocos2d::Sprite&, ActModule&)>;

struct ActModule {
	OPRT_state* state;											// キー情報
	cocos2d::Vec2 vel;											// 左右移動の速度
	bool flipFlg;												// アニメーションの反転判定
	cocos2d::Vec2 map;											// 画面外に出ないようにする
	cocos2d::Vec2 gravity;										// 重力
	cocos2d::Vec2 addGravity;									// 重力に加えていく
	cocos2d::Vec2 jumpVel;										// ジャンプ速度
	cocos2d::Vec2 subtraction;									// ジャンプ値を引いていく

	AnimationType commonAction;									// 共通アクション
	std::string actName;										// プレイヤーのアクション状態(書き込みたい情報)
	BUTTON button;												// キー押下状態

	cocos2d::Vec2 checkPoint1;									// プレイヤーの四隅のどこか(1つ目)
	cocos2d::Vec2 checkPoint2;									// プレイヤーの四隅のどこか(2つ目)
	cocos2d::Vec2 checkPoint3;									// 落下で使うテスト
	cocos2d::Vec2 checkPoint4;									// 落下で使うテスト

	TOUCH_TIMMING touch;										// 操作の方法を決める

	std::list<std::string>blackList;							// ブラックリスト(動いてはいけないやつ)
	std::list<std::string>whiteList;							// ホワイトリスト(動いていいやつ)

	funcActModule runAction;									// モジュールで設定されるアクション
	std::list<funcActModule>act;
	float flame;												// 落下とジャンプにつかうフレーム数
};