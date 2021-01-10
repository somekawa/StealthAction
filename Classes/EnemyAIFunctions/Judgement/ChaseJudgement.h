#pragma once
#include "module/ActModule.h"

// ÌßÚ²Ô°‚ð’Ç‚¢‚©‚¯‚é‚©‚Ì”»’è
struct ChaseJudgement
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};

