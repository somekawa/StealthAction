#include "MoveJudgement.h"
#include "obj/Enemy/Enemy.h"

bool MoveJudgement::Judgement(Enemy* enemy)
{
    // 現在攻撃中であれば移動しない
    if (enemy->IsAttacking())
    {
        enemy->SetMoveType(MoveType::Non);
        return false;
    }
    else
    { 
        //if (!enemy->CheckObjHit())
        {
            // プレイヤーが視界に入っていなければPatrol
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
            // プレイヤーが視界に入ったらchase
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