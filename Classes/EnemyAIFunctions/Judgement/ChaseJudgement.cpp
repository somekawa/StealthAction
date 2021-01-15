#include "ChaseJudgement.h"
#include "obj/Enemy/Enemy.h"
#include "_Debug/_DebugConOut.h"

// true��Ԃ��΂���Action������
bool ChaseJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
	// ��������moveType����Ă���Ă��鎞��true�ł��̂܂ܕԂ��Ȃ���
	// MoveLRAction�̏��������Ȃ��̂�
	if (dynamic_cast<Enemy&>(sprite).GetMoveType() != MoveType::Non)
	{
		return true;
	}
	if (dynamic_cast<Enemy&>(sprite).GetDistance() <= 200.0f)
	{
		TRACE("enemy_chase");
		// enemy��moveType��Chase�ɂ���
		dynamic_cast<Enemy&>(sprite).SetMoveType(MoveType::Chase);
		return true;
	}
	return false;
}
