#include "PatrolJudgement.h"
#include "obj/Enemy/Enemy.h"

// trueを返せばこのActionをする
bool PatrolJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    auto direction = dynamic_cast<Enemy&>(sprite).GetDirection();
    if (!dynamic_cast<Enemy&>(sprite).IsMove(direction))
    {
        return false;
    }
    // もう既にmoveTypeがｾｯﾄされている時はtrueでそのまま返さないと
    // MoveLRActionの処理が回らないので
    if (dynamic_cast<Enemy&>(sprite).GetMoveType() != MoveType::Non)
    {
        return true;
    }

    auto distance = dynamic_cast<Enemy&>(sprite).GetDistance();
    if (distance > 100.0f)
    {
        // enemyのmoveTypeをPatrolにする
        dynamic_cast<Enemy&>(sprite).SetMoveType(MoveType::Patrol);
        return true;
    }
    dynamic_cast<Enemy&>(sprite).SetMoveType(MoveType::Non);

    return false;
}
