#pragma once
#include "module/ActModule.h"

// �v���C���[��ǂ�������A�N�V����
struct ChaseAction
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};