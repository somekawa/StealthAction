#pragma once
#include "module/ActModule.h"

// 落下アクション
struct FallAction
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};