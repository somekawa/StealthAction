#include "ChaseJudgement.h"
#include "obj/Enemy/Enemy.h"
#include "_Debug/_DebugConOut.h"

// true��Ԃ��΂���Action������
bool ChaseJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
	auto direction = dynamic_cast<Enemy&>(sprite).GetDirection();

	if (!dynamic_cast<Enemy&>(sprite).IsMove(direction))
	{
		return false;
	}
	// ��������moveType����Ă���Ă��鎞��true�ł��̂܂ܕԂ��Ȃ���
	// MoveLRAction�̏��������Ȃ��̂�
	if (dynamic_cast<Enemy&>(sprite).GetMoveType() != MoveType::Non)
	{
		return true;
	}

	if (dynamic_cast<Enemy&>(sprite).GetDistance() <= 100.0f && 
		dynamic_cast<Enemy&>(sprite).IsMove(direction))
	{
		TRACE("enemy_chase");
		// enemy��moveType��Chase�ɂ���
		dynamic_cast<Enemy&>(sprite).SetMoveType(MoveType::Chase);
		return true;
	}
	dynamic_cast<Enemy&>(sprite).SetMoveType(MoveType::Non);

	return false;
}
