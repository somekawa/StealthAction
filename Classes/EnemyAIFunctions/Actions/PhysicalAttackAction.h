#pragma once
#include "module/ActModule.h"

struct PhysicalAttackAction
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};

