#include "FallFalling.h"

// 落ちるにつれて速くなる
bool FallFalling::operator()(cocos2d::Sprite & sprite, ActModule & module)
{
	// gravity(不変) * フレーム数(可変)
	sprite.runAction(cocos2d::MoveBy::create(0.0f, cocos2d::Vec2(module.gravity*module.flame)));

	return true;
}
