#pragma once
#include "module/ActModule.h"

// ���񃂁[�h�ɂȂ邩�̔���
struct PatrolJudgement
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};