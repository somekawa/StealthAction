#include "PatrolAction.h"
#include "_Debug/_DebugConOut.h"
#include "obj/Enemy/Enemy.h"

bool PatrolAction::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    TRACE("enemy_Patrol\n");
    const Direction direction = dynamic_cast<Enemy&>(sprite).GetDirection();
    cocos2d::Vec2 vel{ 0.0f,0.0f };

	vel = (direction == Direction::Right ? module.vel : -module.vel);
    dynamic_cast<Enemy&>(sprite).SetAction(module.actName);
    sprite.runAction(cocos2d::MoveBy::create(0.0f, vel));

    return true;
}