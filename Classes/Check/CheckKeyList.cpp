// ëSÇƒíSìñ
#include "CheckKeyList.h"

bool CheckKeyList::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
	if (module.state != nullptr)
	{
		auto oldKey = module.state->GetOldData();
		auto nowKey = module.state->GetNowData();

		// âüÇµÇ¡ÇœÇ»Çµ
		if (module.touch == TOUCH_TIMMING::TOUCHING)
		{
			if (oldKey[static_cast<int>(module.button)] && nowKey[static_cast<int>(module.button)])
			{
				return true;
			}
		}

		// âüÇµÇΩèuä‘
		if (module.touch == TOUCH_TIMMING::ON_TOUCH)
		{
			if (!oldKey[static_cast<int>(module.button)] && nowKey[static_cast<int>(module.button)])
			{
				return true;
			}
		}

		// ó£ÇµÇ¡ÇœÇ»Çµ
		if (module.touch == TOUCH_TIMMING::RELEASED)
		{
			if (!oldKey[static_cast<int>(module.button)] && !nowKey[static_cast<int>(module.button)])
			{
				return true;
			}
		}

		// ó£ÇµÇΩèuä‘
		if (module.touch == TOUCH_TIMMING::ON_RELEASE)
		{
			if (oldKey[static_cast<int>(module.button)] && !nowKey[static_cast<int>(module.button)])
			{
				return true;
			}
		}
	}
	return false;
}