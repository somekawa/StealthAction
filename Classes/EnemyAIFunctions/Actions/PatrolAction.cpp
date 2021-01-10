#include "PatrolAction.h"
#include "_Debug/_DebugConOut.h"

bool PatrolAction::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    TRACE("enemy_Patrol\n");

    return true;
}
