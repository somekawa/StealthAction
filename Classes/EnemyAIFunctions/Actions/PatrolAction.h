#pragma once
#include "module/ActModule.h"

struct PatrolAction
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};

