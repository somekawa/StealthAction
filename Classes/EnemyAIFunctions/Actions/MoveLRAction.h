#pragma once
#include "module/ActModule.h"

// 左右移動アクション
struct MoveLRAction
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};