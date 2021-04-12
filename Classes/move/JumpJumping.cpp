// �S�ĒS��
#include "_Debug/_DebugConOut.h"
#include "JumpJumping.h"

// �オ��ɂ�Ēx���Ȃ�
bool JumpJumping::operator()(cocos2d::Sprite & sprite, ActModule & module)
{
	float tmp = module.jumpVel.y - (9.8f * module.flame);
	if (tmp < -0.1f)
	{
		return false;
	}
	else 
	{
		sprite.runAction(cocos2d::MoveBy::create(0.0f, { module.jumpVel.x, tmp }));
	}
	return true;
}