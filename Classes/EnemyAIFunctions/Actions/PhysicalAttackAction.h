#pragma once
#include "module/ActModule.h"

// �U���A�N�V����
struct PhysicalAttackAction
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};