#pragma once
#include "module/ActModule.h"

struct FallAction
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};

