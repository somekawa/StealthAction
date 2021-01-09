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
	void ActCtl(std::string actName, ActModule& module);
	void update(ActorType type,float sp, cocos2d::Sprite& sprite);
	// Actorのﾀｲﾌﾟ毎のｱｸｼｮﾝ登録用関数の実行
	void RunInitializeActCtl(ActorType type,std::string actName,ActModule& module);
	void InitUpdater(ActorType& type);
private:
	std::map<ActorType, std::function<void(float, cocos2d::Sprite&)>> updater_;
	std::map<std::string, float> _mapFlame;		// フレーム数
	std::map<std::string, ActModule> _mapModule;// モジュール
};