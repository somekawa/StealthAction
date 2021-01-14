#include "BehaviorBaseAI/JudgementTool/SkillAttackJudgement.h"
#include "obj/Enemy/Enemy.h"
bool SkillAttackJudgement::Judgement(Enemy* enemy)
{
	if (enemy->GetDistance() < 400.0f)
	{
		return true;
	}
	return false;
}
