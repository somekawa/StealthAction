#pragma once
#include "module/ActModule.h"

// ����Ӱ�ނɂȂ邩�̔���
struct PatrolJudgement
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};

