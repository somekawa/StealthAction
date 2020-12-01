#include "MoveJudgement.h"
#include "obj/Enemy/Enemy.h"

bool MoveJudgement::Judgement(Enemy* enemy)
{
    // Œ»ÝUŒ‚’†‚Å‚ ‚ê‚ÎˆÚ“®‚µ‚È‚¢
    if (enemy->IsAttacking())
    {
        enemy->SetMoveType(MoveType::Non);
        return false;
    }
    else
    { 
        // ƒvƒŒƒCƒ„[‚ªŽ‹ŠE‚É“ü‚Á‚Ä‚¢‚È‚¯‚ê‚ÎPatrol
        if (enemy->DistanceCalcurator() > visualRange_.x)
        {
            if (enemy->GetMoveType() != MoveType::Patrol)
            {
                enemy->ChangeAnimation("run");
            }
            enemy->SetMoveType(MoveType::Patrol);
            return true;
        }
        // ƒvƒŒƒCƒ„[‚ªŽ‹ŠE‚É“ü‚Á‚½‚çchase
        else if (enemy->DistanceCalcurator() < visualRange_.x)
        {
            if (enemy->GetMoveType() != MoveType::Chase)
            {
                enemy->ChangeAnimation("run");
            }
            enemy->SetMoveType(MoveType::Chase);
            return true;
        }
    }
}