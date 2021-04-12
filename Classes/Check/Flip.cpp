// ’S“–
#include "Flip.h"

bool Flip::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
	sprite.runAction(cocos2d::FlipX::create(module.flipFlg));
	return true;
}