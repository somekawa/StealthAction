#pragma once
#include "module/ActModule.h"

// ���]���邩�ǂ����̔���
struct FlipJudgement
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};