#pragma once
#include "module/ActModule.h"

struct PatrolJudgement
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};

