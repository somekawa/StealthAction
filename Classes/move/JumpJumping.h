#pragma once
#include <cocos2d.h>
#include "module/ActModule.h"

struct JumpJumping
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);		// �オ��ɂ�Ēx���Ȃ�
};