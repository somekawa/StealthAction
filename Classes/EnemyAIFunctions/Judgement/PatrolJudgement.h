#pragma once
#include "module/ActModule.h"

// 巡回ﾓｰﾄﾞになるかの判定
struct PatrolJudgement
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};

