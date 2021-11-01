#include "SkillAttackAction.h"
#include "_Debug/_DebugConOut.h"
#include "obj/Enemy/Enemy.h"

State SkillAttackAction::Run(Enemy* enemy)
{
	TRACE("skillattack");
	//enemy->UseSkill();
	return State::Complete;
}
