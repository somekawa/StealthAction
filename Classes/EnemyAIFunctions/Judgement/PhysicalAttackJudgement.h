#pragma once
#include "module/ActModule.h"

// 攻撃をするかどうかの判定
struct PhysicalAttackJudgement
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};