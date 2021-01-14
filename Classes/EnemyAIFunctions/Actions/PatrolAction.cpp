#include "PatrolAction.h"
#include "_Debug/_DebugConOut.h"
#include "obj/Enemy/Enemy.h"

bool PatrolAction::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    TRACE("enemy_Patrol\n");
    auto direction = dynamic_cast<Enemy&>(sprite).GetDirection();
    auto vel = cocos2d::Vec2{ 0.0f,0.0f };
    if (direction == Direction::Right)
    {
        vel = module.vel;
    }
    if (direction == Direction::Left)
    {
        vel = -module.vel;
    }
    sprite.runAction(cocos2d::MoveBy::create(0.0f, vel));

    return true;
}
