#include "CheckKeyList.h"

bool CheckKeyList::operator()(cocos2d::Sprite & sprite, ActModule & module)
{
	auto oldKey = module.state->GetOldData();
	auto nowKey = module.state->GetNowData();

	// 押しっぱなし
	if (module.touch == TOUCH_TIMMING::TOUCHING)
	{
		if (oldKey[static_cast<int>(module.button)] && nowKey[static_cast<int>(module.button)])
		{
			return true;
		}
	}

	// 押した瞬間
	if (module.touch == TOUCH_TIMMING::ON_TOUCH)
	{
		if (!oldKey[static_cast<int>(module.button)] && nowKey[static_cast<int>(module.button)])
		{
			return true;
		}
	}

	// 離しっぱなし
	if (module.touch == TOUCH_TIMMING::RELEASED)
	{
		if (!oldKey[static_cast<int>(module.button)] && !nowKey[static_cast<int>(module.button)])
		{
			return true;
		}
	}

	// 離した瞬間
	if (module.touch == TOUCH_TIMMING::ON_RELEASE)
	{
		if (oldKey[static_cast<int>(module.button)] && !nowKey[static_cast<int>(module.button)])
		{
			return true;
		}
	}

	return false;
}
