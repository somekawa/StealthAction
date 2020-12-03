#pragma once
#include <cocos2d.h>
#include "module/ActModule.h"

struct Attack
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
	float cntTest = 0.0f;
};