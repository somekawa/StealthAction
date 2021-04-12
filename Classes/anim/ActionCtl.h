#pragma once
#include <cocos2d.h>
#include <utility>
#include <vector>
#include <map>
#include <array>
#include <functional>
#include <string>
#include "input/OPRT_state.h"
#include "module/ActModule.h"
#include "obj/ActorType.h"
#include "../AnimationEnum/AnimationType.h"

// first:runaction,second:check関連
using xmlMap = std::map<AnimationType, std::pair<std::string, std::vector<std::string>>>;
// キー:runactionまたはcheck関連の構造体名
using actFunc = std::map<std::string, std::function<bool(cocos2d::Sprite& sprite, ActModule& module)>>;

class ActionCtl
{
public:
	ActionCtl();
	~ActionCtl();
	// ActCtlの更新(タイプ別でUpdateを回す)
	void Update(ActorType type,cocos2d::Sprite& sprite);

	// Actorのタイプ毎のアクション登録用関数の実行
	void InitAct(ActorType type, AnimationType actName,ActModule& module);
	// 個々のアクターのmoduleの更新関数の登録
	void InitUpdater(ActorType& type);
private:
	// 個々のアクター毎の更新関数(返り値はactFlgのboolにしたい)
	std::map<ActorType, std::function<void(cocos2d::Sprite&)>> updater_;
	std::map<AnimationType, float> mapFlame_;	  // フレーム数
	std::map<AnimationType, ActModule> mapModule_;// モジュール

	actFunc checkStructFunc_;	// チェック関係の構造体を登録
	actFunc plActStructFunc_;	// プレイヤーのモジュール関係の構造体を登録
	actFunc enActStructFunc_;	// エネミーのモジュール関係の構造体を登録

	void XmlPurser(std::string fileName,xmlMap& xmlmap);	// xmlデータを読み込む
	xmlMap plXmlData_;										// プレイヤーに関するデータを保存するやつ
	xmlMap enXmlData_;										// エネミーに関するデータを保存するやつ
};