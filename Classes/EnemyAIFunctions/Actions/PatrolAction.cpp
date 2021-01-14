#include "PatrolAction.h"
#include "_Debug/_DebugConOut.h"

bool PatrolAction::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    TRACE("enemy_Patrol\n");
    sprite.runAction(cocos2d::MoveBy::create(0.0f, cocos2d::Vec2(module.vel)));
    return true;
}
