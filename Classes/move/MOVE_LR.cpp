// ‘S‚Ä’S“–
#include <cocos2d.h>
#include "_Debug/_DebugConOut.h"
#include "obj/Player.h"
#include "Move_LR.h"

bool Move_LR::operator()(cocos2d::Sprite & sprite, ActModule & module)
{
	// dark‚Ì‚Æ‚«‚ÍˆÚ“®—Í‚ªã‚ª‚é
	float speed = dynamic_cast<Player&>(sprite).GetRunSpeedUp();
	if (module.button == BUTTON::LEFT)
	{
		speed *= -1.0f;
	}
	sprite.runAction(cocos2d::MoveBy::create(0.0f, { module.vel.x + speed,module.vel.y }));
	return true;
}