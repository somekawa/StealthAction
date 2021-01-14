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
        //if (!enemy->CheckObjHit())
        {
            // ƒvƒŒƒCƒ„[‚ªŽ‹ŠE‚É“ü‚Á‚Ä‚¢‚È‚¯‚ê‚ÎPatrol
            if (enemy->GetDistance() > enemy->GetVisionRange().chase_)
            {
                if (enemy->GetType() == ActorType::Assassin ||
                    enemy->GetType() == ActorType::BigCultist)
                {
                    if (enemy->GetMoveType() != MoveType::Patrol)
                    {
                        enemy->ChangeAnimation("run");
                    }
                }
                else
                {
                    if (enemy->GetMoveType() != MoveType::Patrol)
                    {
                        enemy->ChangeAnimation("walk");
                    }
                }
                enemy->SetMoveType(MoveType::Patrol);
                return true;
            }
            // ƒvƒŒƒCƒ„[‚ªŽ‹ŠE‚É“ü‚Á‚½‚çchase
            else if (enemy->GetDistance() < enemy->GetVisionRange().chase_)
            {
                if (enemy->GetType() == ActorType::Assassin ||
                    enemy->GetType() == ActorType::BigCultist)
                {
                    if (enemy->GetMoveType() != MoveType::Chase)
                    {
                        enemy->ChangeAnimation("run");
                    }
                }
                else
                {
                    if (enemy->GetMoveType() != MoveType::Chase)
                    {
                        enemy->ChangeAnimation("walk");
                    }
                }
                enemy->SetMoveType(MoveType::Chase);
                return true;
            }
        }
    }
}