#pragma once
#include "cocos2d.h"
#include "input/OPRT_state.h"
#include "module/ActModule.h"
#include "obj/ActorType.h"
#include <map>
#include <array>
#include <functional>

class ActionCtl
{
public:
	ActionCtl();
	~ActionCtl();
	// これはもう使用しない
	// RunInitializeActCtl関数でﾀｲﾌﾟ別のmoduleを登録可能にしている
	void ActCtl(std::string actName, ActModule& module);
	// ActCtlの更新(ﾀｲﾌﾟ別でupdateを回したい)
	void update(ActorType type,float sp, cocos2d::Sprite& sprite);
	// Actorのﾀｲﾌﾟ毎のｱｸｼｮﾝ登録用関数の実行
	void RunInitializeActCtl(ActorType type,std::string actName,ActModule& module);
	// 個々のｱｸﾀｰのmoduleの更新関数の登録
	void InitUpdater(ActorType& type);
private:
	// 個々のｱｸﾀｰ毎の更新関数
	std::map<ActorType, std::function<void(float, cocos2d::Sprite&)>> updater_;
	std::map<std::string, float> _mapFlame;		// フレーム数
	std::map<std::string, ActModule> _mapModule;// モジュール
};