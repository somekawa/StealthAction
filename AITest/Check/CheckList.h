#pragma once

#include "module/ActModule.h"

struct CheckList
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};
