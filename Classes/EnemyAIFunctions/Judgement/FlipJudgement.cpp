#include "FlipJudgement.h"
#include "obj/Enemy/Enemy.h"

// true��Ԃ��΂���Action�����s����
bool FlipJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    // �����݌����Ă��������1�t���[���O�Ɍ����Ă�������ƈ��������s
    if (dynamic_cast<Enemy&>(sprite).GetDirection() != dynamic_cast<Enemy&>(sprite).GetOldDirection())
    {
        return true;
    }
    // �����ꍇ�͎��s���Ȃ�(�����Ă�������Ɠ����ꍇ�͔��]���Ȃ��Ă悢)
    return false;
}