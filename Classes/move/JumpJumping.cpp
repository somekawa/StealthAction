#include "JumpJumping.h"

// ã‚ª‚é‚É‚Â‚ê‚Ä’x‚­‚È‚é
bool JumpJumping::operator()(cocos2d::Sprite & sprite, ActModule & module)
{
	if (module.jumpVel.y - (9.8f * module.flame) < -0.1f)
	{
		return false;
	}
	else{
		sprite.runAction(cocos2d::MoveBy::create(0.0f, cocos2d::Vec2(module.jumpVel.x, module.jumpVel.y - (9.8f * module.flame))));
	}
	return true;
}
