#pragma once
#include "module/ActModule.h"

// ��ڲ԰��ǂ������鱸���
struct ChaseAction
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};

