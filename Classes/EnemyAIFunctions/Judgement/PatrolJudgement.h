#pragma once
#include "module/ActModule.h"

// „‰ñƒ‚[ƒh‚É‚È‚é‚©‚Ì”»’è
struct PatrolJudgement
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};