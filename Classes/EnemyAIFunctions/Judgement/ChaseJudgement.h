#pragma once
#include "module/ActModule.h"

// ﾌﾟﾚｲﾔｰを追いかけるかの判定
struct ChaseJudgement
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};

