#include "PatrolJudgement.h"
#include "obj/Enemy/Enemy.h"

// true��Ԃ��΂���Action������
bool PatrolJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    auto direction = dynamic_cast<Enemy&>(sprite).GetDirection();
    if (!dynamic_cast<Enemy&>(sprite).IsMove(direction))
    {
        return false;
    }
    // ��������moveType����Ă���Ă��鎞��true�ł��̂܂ܕԂ��Ȃ���
    // MoveLRAction�̏��������Ȃ��̂�
    if (dynamic_cast<Enemy&>(sprite).GetMoveType() != MoveType::Non)
    {
        return true;
    }

    auto distance = dynamic_cast<Enemy&>(sprite).GetDistance();
    if (distance > 100.0f)
    {
        // enemy��moveType��Patrol�ɂ���
        dynamic_cast<Enemy&>(sprite).SetMoveType(MoveType::Patrol);
        return true;
    }
    dynamic_cast<Enemy&>(sprite).SetMoveType(MoveType::Non);

    return false;
}
