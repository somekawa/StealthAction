#pragma once
#include "module/ActModule.h"

// �摜�̍��E���]�����
struct FlipAction
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);
};

