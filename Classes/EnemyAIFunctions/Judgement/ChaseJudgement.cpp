#include "ChaseJudgement.h"
#include "obj/Enemy/Enemy.h"
#include "_Debug/_DebugConOut.h"

// trueを返せばこのActionをする
bool ChaseJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
	// もう既にmoveTypeがｾｯﾄされている時はtrueでそのまま返さないと
	// MoveLRActionの処理が回らないので
	if (dynamic_cast<Enemy&>(sprite).GetMoveType() != MoveType::Non)
	{
		return true;
	}
	if (dynamic_cast<Enemy&>(sprite).GetDistance() <= 200.0f)
	{
		TRACE("enemy_chase");
		// enemyのmoveTypeをChaseにする
		dynamic_cast<Enemy&>(sprite).SetMoveType(MoveType::Chase);
		return true;
	}
	return false;
}
