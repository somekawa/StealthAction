#pragma once
#include "module/ActModule.h"

// 巡回アクション
struct PatrolAction
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};