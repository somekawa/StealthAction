#include <cocos2d.h>
#include "MOVE_LR.h"
#include "_Debug/_DebugConOut.h"

bool MOVE_LR::operator()(cocos2d::Sprite & sprite, ActModule & module)
{
	sprite.runAction(cocos2d::MoveBy::create(0.0f, cocos2d::Vec2(module.vel)));
	TRACE("MOVE_LR\n");
	return true;
}
