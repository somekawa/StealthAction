#pragma once
#include "module/ActModule.h"

// ����A�N�V����
struct PatrolAction
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};