#pragma once
#include "module/ActModule.h"

// ���E�ړ��A�N�V����
struct MoveLRAction
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};