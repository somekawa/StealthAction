#pragma once
#include "module/ActModule.h"

struct FlipJudgement
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};

