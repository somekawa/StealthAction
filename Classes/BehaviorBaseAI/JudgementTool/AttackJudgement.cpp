#include "BehaviorBaseAI/JudgementTool/AttackJudgement.h"
#include "obj/Enemy/Enemy.h"

bool AttackJudgement::Judgement(Enemy* enemy)
{
	if (enemy->DistanceCalcurator() < enemy->GetVisionRange().attack_)
	{
		if (!enemy->IsAttacking())
		{
			enemy->ChangeAnimation("attack");
			enemy->SetIsAttacking(true);
		}
		return true;
	}
	return false;
}
