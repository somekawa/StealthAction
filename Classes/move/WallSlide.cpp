#include "_Debug/_DebugConOut.h"
#include "WallSlide.h"

bool WallSlide::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
	sprite.runAction(cocos2d::FlipX::create(module.flipFlg));
	sprite.runAction(cocos2d::MoveBy::create(0.0f, cocos2d::Vec2(module.gravity * module.flame)));
	TRACE("Wall_Slide\n");
	return true;
}
