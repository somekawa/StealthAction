#pragma once
#include "module/ActModule.h"

// プレイヤーを追いかけるかの判定
struct ChaseJudgement
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};