#include "FlipJudgement.h"
#include "obj/Enemy/Enemy.h"

// trueを返せばこのActionを実行する
bool FlipJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    // 今現在向いている向きが1フレーム前に向いている向きと違ったら実行
    if (dynamic_cast<Enemy&>(sprite).GetDirection() != dynamic_cast<Enemy&>(sprite).GetOldDirection())
    {
        return true;
    }
    // 同じ場合は実行しない(向いている向きと同じ場合は反転しなくてよい)
    return false;
}