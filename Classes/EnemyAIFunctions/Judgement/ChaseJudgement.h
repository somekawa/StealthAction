#pragma once
#include "module/ActModule.h"

struct ChaseJudgement
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};

