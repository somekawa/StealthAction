#include "obj/Actor.h"
#include "CheckList.h"

bool CheckList::operator()(cocos2d::Sprite & sprite, ActModule & module)
{
	// �z���C�g���X�g�o�^��Check
	for (auto data : module.whiteList)
	{
		if (data == (dynamic_cast<Actor&>(sprite)).GetAction())
		{
			return true;
		}
	}

	// �u���b�N���X�g�o�^��Check
	for (auto data : module.blackList)
	{
		if (data == (dynamic_cast<Actor&>(sprite)).GetAction())
		{
			return false;
		}
	}
	return true;
}
