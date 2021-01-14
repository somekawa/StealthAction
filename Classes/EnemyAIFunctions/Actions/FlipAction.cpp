#include "FlipAction.h"
#include "_Debug/_DebugConOut.h"

bool FlipAction::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    TRACE("enemy_Flip\n");
    sprite.runAction(cocos2d::FlipX::create(module.flipFlg));
    return true;
}
