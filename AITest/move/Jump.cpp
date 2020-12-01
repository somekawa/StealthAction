#include "Jump.h"
#include "_Debug/_DebugConOut.h"

bool Jump::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
	TRACE("JUMP\n");
	if (!module.jumpFlg)
	{
		module.jumpFlg = true;
	}
	else
	{
		module.jumpFlg = false;
	}
	return true;
}