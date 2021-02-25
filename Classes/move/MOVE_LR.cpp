#include <cocos2d.h>
#include "Move_LR.h"
#include "obj/Player.h"
#include "_Debug/_DebugConOut.h"

bool Move_LR::operator()(cocos2d::Sprite & sprite, ActModule & module)
{
	auto plColor = dynamic_cast<Player&>(sprite).GetPlayerColor();
	float speed = 0;
	// dark‚Ì‚Æ‚«‚ÍˆÚ“®—Í‚ªã‚ª‚é
	if (plColor == "player_Dark_")
	{
		module.button == BUTTON::RIGHT ? speed = 2 : speed = -2;
	}
	sprite.runAction(cocos2d::MoveBy::create(0.0f, cocos2d::Vec2(module.vel.x + speed,module.vel.y)));

	TRACE("Move_LR\n");
	return true;
}