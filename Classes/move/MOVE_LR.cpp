#include <cocos2d.h>
#include "MOVE_LR.h"

bool MOVE_LR::operator()(cocos2d::Sprite & sprite, ActModule & module)
{
	sprite.runAction(cocos2d::MoveBy::create(0.0f, cocos2d::Vec2(module.vel)));
	return true;
}
