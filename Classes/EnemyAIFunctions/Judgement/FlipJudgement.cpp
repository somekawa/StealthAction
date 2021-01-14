#include "FlipJudgement.h"
#include "obj/Enemy/Enemy.h"

// trueを返せばこのActionをする
bool FlipJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    // 今現在向いている向き情報
    auto nowDirection = dynamic_cast<Enemy&>(sprite).GetDirection();
    // 1ﾌﾚｰﾑ前の向き情報
    auto oldDirection = dynamic_cast<Enemy&>(sprite).GetOldDirection();
    // 今現在向いている向きが1ﾌﾚｰﾑ前に向いている向きと違ったら実行
    if (nowDirection != oldDirection)
    {
        return true;
    }
    // 同じ場合は実行しない(向いている向きと同じ場合は反転しなくてよい)
    return false;
}
