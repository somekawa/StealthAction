#include "PhysicalAttackJudgement.h"
#include "obj/Enemy/Enemy.h"

bool PhysicalAttackJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
	// ‚Ü‚¸‹——£‚ğŒ©‚é
	auto distance = dynamic_cast<Enemy&>(sprite).GetDistance();
	// ‹——£‚ªUŒ‚”ÍˆÍ“à‚¾‚Á‚½‚ç
	if (distance <= dynamic_cast<Enemy&>(sprite).GetVisionRange().attack_ - 10.0f)
	{
		// UŒ‚±¸¼®İ‚ÌÀs
		return true;
	}
	// ‚»‚êˆÈŠO‚Å‚ ‚ê‚ÎUŒ‚‚µ‚È‚¢
	// UŒ‚Ì×¸Ş‚ğfalse‚É
	dynamic_cast<Enemy&>(sprite).SetIsAttacking(false);
	return false;
}
