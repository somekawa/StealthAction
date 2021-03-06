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
        // プレイヤーが視界に入っていなければPatrol
        if (enemy->DistanceCalcurator() > visualRange_.x)
        {
            if (enemy->GetMoveType() != MoveType::Patrol)
            {
                enemy->ChangeAnimation("run");
            }
            enemy->SetMoveType(MoveType::Patrol);
            return true;
        }
        // プレイヤーが視界に入ったらchase
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