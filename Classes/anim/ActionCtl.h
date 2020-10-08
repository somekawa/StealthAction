#pragma once
#include "cocos2d.h"
#include "input/DIR_ID.h"
#include "input/OPRT_state.h"
#include "module/ActModule.h"
#include <map>

class ActionCtl
{
public:
	ActionCtl();
	~ActionCtl();
	void ActCtl(std::string actName, ActModule& module);
	void update(float sp, cocos2d::Sprite& sprite);
private:
	std::map<std::string, float> _mapFlame;		// フレーム数
	std::map<std::string, ActModule> _mapModule;// モジュール
	bool jumpFlg = false;
};

