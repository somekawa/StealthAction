#pragma once
#include "module/ActModule.h"

struct FlipAction
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};

