#pragma once
#include "module/ActModule.h"

// ﾌﾟﾚｲﾔｰを追いかけるｱｸｼｮﾝ
struct ChaseAction
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};

