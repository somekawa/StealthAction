#include "ChaseJudgement.h"
#include "obj/Enemy/Enemy.h"
#include "_Debug/_DebugConOut.h"

// true‚ğ•Ô‚¹‚Î‚±‚ÌAction‚ğ‚·‚é
bool ChaseJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
	// ‚à‚¤Šù‚ÉmoveType‚ª¾¯Ä‚³‚ê‚Ä‚¢‚é‚Ítrue‚Å‚»‚Ì‚Ü‚Ü•Ô‚³‚È‚¢‚Æ
	// MoveLRAction‚Ìˆ—‚ª‰ñ‚ç‚È‚¢‚Ì‚Å
	if (dynamic_cast<Enemy&>(sprite).GetMoveType() != MoveType::Non)
	{
		return true;
	}
	if (dynamic_cast<Enemy&>(sprite).GetDistance() <= 200.0f)
	{
		TRACE("enemy_chase");
		// enemy‚ÌmoveType‚ğChase‚É‚·‚é
		dynamic_cast<Enemy&>(sprite).SetMoveType(MoveType::Chase);
		return true;
	}
	return false;
}
