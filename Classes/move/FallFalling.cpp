#include "FallFalling.h"
#include "_Debug/_DebugConOut.h"

// ������ɂ�đ����Ȃ�
bool FallFalling::operator()(cocos2d::Sprite & sprite, ActModule & module)
{
	// gravity(�s��) * �t���[����(��)
	sprite.runAction(cocos2d::MoveBy::create(0.0f, cocos2d::Vec2(module.gravity*module.flame)));
	TRACE("FALL\n");
	return true;
}
