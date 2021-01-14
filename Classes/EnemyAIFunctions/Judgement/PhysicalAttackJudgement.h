#pragma once
#include "module/ActModule.h"

struct PhysicalAttackJudgement
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};

