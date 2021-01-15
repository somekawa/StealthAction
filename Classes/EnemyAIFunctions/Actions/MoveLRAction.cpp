#include "MoveLRAction.h"
#include "obj/Enemy/Enemy.h"
#include "_Debug/_DebugConOut.h"

bool MoveLRAction::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    auto direction = dynamic_cast<Enemy&>(sprite).GetDirection();
    auto vel = cocos2d::Vec2{ 0.0f,0.0f };
    dynamic_cast<Enemy&>(sprite).SetAction(module.actName);
    // PatrolJudgement‚ªtrue‚É‚È‚Á‚½ê‡‚Ìˆ—
    if (dynamic_cast<Enemy&>(sprite).GetMoveType() == MoveType::Patrol)
    {
        TRACE("enemy_patrol");
        if (direction == Direction::Right)
        {
            vel = module.vel;
        }
        if (direction == Direction::Left)
        {
            vel = -module.vel;
        }
        sprite.runAction(cocos2d::MoveBy::create(0.0f, vel));
    }
    // chaseJudgement‚ªtrue‚É‚È‚Á‚½ê‡‚Ìˆ—
    else if (dynamic_cast<Enemy&>(sprite).GetMoveType() == MoveType::Chase)
    {
        TRACE("enemy_chase");
        if (direction == Direction::Right)
        {
            vel = module.vel * 2;
        }
        if (direction == Direction::Left)
        {
            vel = -module.vel * 2;
        }
        sprite.runAction(cocos2d::MoveBy::create(0.0f, vel));
    }
    return false;
}
