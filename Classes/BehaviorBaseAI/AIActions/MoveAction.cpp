#include "MoveAction.h"
#include "_Debug/_DebugConOut.h"
#include "obj/Enemy/Enemy.h"

State MoveAction::Run(Enemy* enemy)
{
	switch (enemy->GetMoveType())
	{
	case MoveType::Chase:
		enemy->Chase();
		TRACE("Chase");
		break;
	case MoveType::Patrol:
		enemy->Patrol();
		TRACE("Patrol");
		break;
	default:
		break;
	}
    return State::Complete;
}
