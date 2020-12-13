#include "FallFalling.h"
#include "_Debug/_DebugConOut.h"

// —Ž‚¿‚é‚É‚Â‚ê‚Ä‘¬‚­‚È‚é
bool FallFalling::operator()(cocos2d::Sprite & sprite, ActModule & module)
{
	// gravity(•s•Ï) * ƒtƒŒ[ƒ€”(‰Â•Ï)
	// —Ž‰º’†‚ÉAttack‚ª‰Ÿ‚³‚ê‚½‚çˆêuÃŽ~‚·‚é‚æ‚¤‚É
	if (module.stopCnt > 0)
	{
		return true;
	}
	sprite.runAction(cocos2d::MoveBy::create(0.0f, cocos2d::Vec2(module.gravity*module.flame)));
	TRACE("FALL\n");
	return true;
}