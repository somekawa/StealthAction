#include "obj/Actor.h"
#include "CheckList.h"

bool CheckList::operator()(cocos2d::Sprite & sprite, ActModule & module)
{
	// ホワイトリスト登録物Check
	for (auto data : module.whiteList)
	{
		if (data == (dynamic_cast<Actor&>(sprite)).GetAction())
		{
			return true;
		}
	}

	// ブラックリスト登録物Check
	for (auto data : module.blackList)
	{
		if (data == (dynamic_cast<Actor&>(sprite)).GetAction())
		{
			return false;
		}
	}
	return true;
}
