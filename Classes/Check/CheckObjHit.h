#pragma once
#include "module/ActModule.h"

struct CheckObjHit
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};