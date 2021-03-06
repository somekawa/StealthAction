#include "FallFalling.h"
#include "_Debug/_DebugConOut.h"

// 落ちるにつれて速くなる
bool FallFalling::operator()(cocos2d::Sprite & sprite, ActModule & module)
{
	// gravity(不変) * フレーム数(可変)
	// 落下中にAttackが押されたら一瞬静止するように
	if (module.stopCnt > 0)
	{
		return true;
	}
	sprite.runAction(cocos2d::MoveBy::create(0.0f, cocos2d::Vec2(module.gravity*module.flame)));
	TRACE("FALL\n");
	return true;
}