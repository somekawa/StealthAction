#include "PatrolJudgement.h"
#include "obj/Enemy/Enemy.h"

// trueを返せばこのActionを実行する
bool PatrolJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    if (!dynamic_cast<Enemy&>(sprite).IsMove(dynamic_cast<Enemy&>(sprite).GetDirection()))
    {
        return false;
    }

    // 既にmoveTypeがセットされている時はtrueで返す
    if (dynamic_cast<Enemy&>(sprite).GetMoveType() != MoveType::Non)
    {
        return true;
    }

    if (dynamic_cast<Enemy&>(sprite).GetDistance() > 100.0f)
    {
        // enemyのmoveTypeをPatrolにする
        dynamic_cast<Enemy&>(sprite).SetMoveType(MoveType::Patrol);
        return true;
    }
    dynamic_cast<Enemy&>(sprite).SetMoveType(MoveType::Non);

    return false;
}