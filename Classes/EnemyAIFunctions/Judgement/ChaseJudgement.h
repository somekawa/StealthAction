#pragma once
#include "module/ActModule.h"

// �v���C���[��ǂ������邩�̔���
struct ChaseJudgement
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};