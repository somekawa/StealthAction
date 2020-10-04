#include "FallFalling.h"
#include "_Debug/_DebugConOut.h"

// —‚¿‚é‚É‚Â‚ê‚Ä‘¬‚­‚È‚é
bool FallFalling::operator()(cocos2d::Sprite & sprite, ActModule & module)
{
	// gravity(•s•Ï) * ƒtƒŒ[ƒ€”(‰Â•Ï)
	sprite.runAction(cocos2d::MoveBy::create(0.0f, cocos2d::Vec2(module.gravity*module.flame)));
	TRACE("FALL\n");
	return true;
}
