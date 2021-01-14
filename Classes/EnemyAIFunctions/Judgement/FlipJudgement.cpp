#include "FlipJudgement.h"
#include "obj/Enemy/Enemy.h"

// true��Ԃ��΂���Action������
bool FlipJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    // �����݌����Ă���������
    auto nowDirection = dynamic_cast<Enemy&>(sprite).GetDirection();
    // 1�ڰёO�̌������
    auto oldDirection = dynamic_cast<Enemy&>(sprite).GetOldDirection();
    // �����݌����Ă��������1�ڰёO�Ɍ����Ă�������ƈ��������s
    if (nowDirection != oldDirection)
    {
        return true;
    }
    // �����ꍇ�͎��s���Ȃ�(�����Ă�������Ɠ����ꍇ�͔��]���Ȃ��Ă悢)
    return false;
}
