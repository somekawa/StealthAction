#pragma once
#include "module/ActModule.h"

// ”½“]‚·‚é‚©‚Ç‚¤‚©‚Ì”»’è
struct FlipJudgement
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};