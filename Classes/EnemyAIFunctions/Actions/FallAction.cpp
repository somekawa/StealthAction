#include "FallAction.h"
#include "obj/Enemy/Enemy.h"

bool FallAction::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    dynamic_cast<Enemy&>(sprite).SetAction(module.actName);
    sprite.runAction(cocos2d::MoveBy::create(0.0f, cocos2d::Vec2(module.gravity * module.flame)));
    return true;
}
