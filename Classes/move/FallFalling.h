#pragma once
#include <cocos2d.h>
#include "module/ActModule.h"

struct FallFalling
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);		// 落ちるにつれて速くなる
};