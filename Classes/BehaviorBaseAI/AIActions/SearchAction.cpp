#include "SearchAction.h"
#include "obj/Enemy/Enemy.h"

State SearchAction::Run(Enemy* enemy)
{
	enemy->SearchPlayer();
	return State::Complete;
}
