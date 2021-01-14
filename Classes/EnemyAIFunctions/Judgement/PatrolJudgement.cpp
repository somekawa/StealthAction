#include "PatrolJudgement.h"
#include "obj/Enemy/Enemy.h"

// trueを返せばこのActionをする
bool PatrolJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    auto distance = dynamic_cast<Enemy&>(sprite).GetDistance();
    if (distance >= 100.0f)
    {
        dynamic_cast<Enemy&>(sprite).SetAction(module.actName);

        return true;
    }
    return false;
}
