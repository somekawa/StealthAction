#pragma once
#include "module/ActModule.h"

// �U�������邩�ǂ����̔���
struct PhysicalAttackJudgement
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};