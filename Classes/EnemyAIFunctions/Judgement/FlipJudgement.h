#pragma once
#include "module/ActModule.h"

// 反転するかどうかの判定
struct FlipJudgement
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};

