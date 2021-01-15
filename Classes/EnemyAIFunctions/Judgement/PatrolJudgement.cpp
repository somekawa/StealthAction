#include "PatrolJudgement.h"
#include "obj/Enemy/Enemy.h"

// true‚ğ•Ô‚¹‚Î‚±‚ÌAction‚ğ‚·‚é
bool PatrolJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    // ‚à‚¤Šù‚ÉmoveType‚ª¾¯Ä‚³‚ê‚Ä‚¢‚é‚Ítrue‚Å‚»‚Ì‚Ü‚Ü•Ô‚³‚È‚¢‚Æ
    // MoveLRAction‚Ìˆ—‚ª‰ñ‚ç‚È‚¢‚Ì‚Å
    if (dynamic_cast<Enemy&>(sprite).GetMoveType() != MoveType::Non)
    {
        return true;
    }
    auto distance = dynamic_cast<Enemy&>(sprite).GetDistance();
    if (distance > 100.0f)
    {
        // enemy‚ÌmoveType‚ğPatrol‚É‚·‚é
        dynamic_cast<Enemy&>(sprite).SetMoveType(MoveType::Patrol);
        return true;
    }
    dynamic_cast<Enemy&>(sprite).SetMoveType(MoveType::Non);
    return false;
}
