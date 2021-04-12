#include "MoveLRAction.h"
#include "obj/Enemy/Enemy.h"
#include "_Debug/_DebugConOut.h"

bool MoveLRAction::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    const Direction direction = dynamic_cast<Enemy&>(sprite).GetDirection();
    cocos2d::Vec2 vel{ 0.0f,0.0f };
    dynamic_cast<Enemy&>(sprite).SetAction(module.actName);

    if (dynamic_cast<Enemy&>(sprite).GetMoveType() == MoveType::Patrol)
    {
		// PatrolJudgementがtrueになった場合の処理
        TRACE("enemy_patrol");
		vel = (direction == Direction::Right ? module.vel : -module.vel);
        sprite.runAction(cocos2d::MoveBy::create(0.0f, vel));
    }
    else if (dynamic_cast<Enemy&>(sprite).GetMoveType() == MoveType::Chase)
    {
		// chaseJudgementがtrueになった場合の処理
        TRACE("enemy_chase");
		vel = (direction == Direction::Right ? module.vel * 2 : -module.vel * 2);
        sprite.runAction(cocos2d::MoveBy::create(0.0f, vel));
    }
	else
	{
		// 何も処理を行わない
	}
    return false;
}