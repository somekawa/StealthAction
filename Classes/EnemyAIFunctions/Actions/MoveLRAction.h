#pragma once
#include "module/ActModule.h"

struct MoveLRAction
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};

