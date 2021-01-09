#pragma once
#include "module/ActModule.h"

struct ChaseAction
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};

