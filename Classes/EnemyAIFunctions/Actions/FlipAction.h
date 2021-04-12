#pragma once
#include "module/ActModule.h"

// 画像の左右反転アクション
struct FlipAction
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};