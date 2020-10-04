#pragma once
#include <cocos2d.h>
#include "module/ActModule.h"

struct Jump
{
	bool operator()(cocos2d::Sprite& sprite, ActModule& module);		
};