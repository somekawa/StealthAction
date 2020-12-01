#pragma once
#include "module/ActModule.h"

struct CheckKeyList
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};

