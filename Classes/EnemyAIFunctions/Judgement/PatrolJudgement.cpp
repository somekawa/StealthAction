#include "PatrolJudgement.h"
#include "obj/Enemy/Enemy.h"

// true��Ԃ��΂���Action�����s����
bool PatrolJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    if (!dynamic_cast<Enemy&>(sprite).IsMove(dynamic_cast<Enemy&>(sprite).GetDirection()))
    {
        return false;
    }

    // ����moveType���Z�b�g����Ă��鎞��true�ŕԂ�
    if (dynamic_cast<Enemy&>(sprite).GetMoveType() != MoveType::Non)
    {
        return true;
    }

    if (dynamic_cast<Enemy&>(sprite).GetDistance() > 100.0f)
    {
        // enemy��moveType��Patrol�ɂ���
        dynamic_cast<Enemy&>(sprite).SetMoveType(MoveType::Patrol);
        return true;
    }
    dynamic_cast<Enemy&>(sprite).SetMoveType(MoveType::Non);

    return false;
}