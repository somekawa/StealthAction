// �S�ĒS��
#include "_Debug/_DebugConOut.h"
#include "WallSlide.h"

bool WallSlide::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
	// �ǂɂ͂���A�ɂ₩�ɉ������Ă���
	sprite.runAction(cocos2d::FlipX::create(module.flipFlg));
	sprite.runAction(cocos2d::MoveBy::create(0.0f, { module.gravity * module.flame }));
	return true;
}