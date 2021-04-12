// 全て担当
#include "_Debug/_DebugConOut.h"
#include "WallSlide.h"

bool WallSlide::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
	// 壁にはりつき、緩やかに下がっていく
	sprite.runAction(cocos2d::FlipX::create(module.flipFlg));
	sprite.runAction(cocos2d::MoveBy::create(0.0f, { module.gravity * module.flame }));
	return true;
}