#include "NormalAttack.h"
#include "obj/Enemy/Enemy.h"
#include "_Debug/_DebugConOut.h"

State NormalAttack::Run(Enemy* enemy)
{
    TRACE("NormalAttack");
    enemy->NormalAttack();
    return State::Run;
}
