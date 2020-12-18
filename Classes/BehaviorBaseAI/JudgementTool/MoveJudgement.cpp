#include "MoveJudgement.h"
#include "obj/Enemy/Enemy.h"

bool MoveJudgement::Judgement(Enemy* enemy)
{
    // ���ݍU�����ł���Έړ����Ȃ�
    if (enemy->IsAttacking())
    {
        enemy->SetMoveType(MoveType::Non);
        return false;
    }
    else
    { 
        //if (!enemy->CheckObjHit())
        {
            // �v���C���[�����E�ɓ����Ă��Ȃ����Patrol
            if (enemy->DistanceCalcurator() > enemy->GetVisionRange().chase_)
            {
                if (enemy->GetMoveType() != MoveType::Patrol)
                {
                    enemy->ChangeAnimation("run");
                }
                enemy->SetMoveType(MoveType::Patrol);
                return true;
            }
            // �v���C���[�����E�ɓ�������chase
            else if (enemy->DistanceCalcurator() < enemy->GetVisionRange().chase_)
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
}