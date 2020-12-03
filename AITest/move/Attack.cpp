#include <cocos2d.h>
#include "Attack.h"
#include "_Debug/_DebugConOut.h"

bool Attack::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
	TRACE("ATTACK\n");
	return true;
}
