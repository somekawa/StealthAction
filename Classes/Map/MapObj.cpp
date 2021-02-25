#include "MapObj.h"
#include "../obj/Player.h"

USING_NS_CC;

MapObj::MapObj()
{
}

MapObj::~MapObj()
{
}

bool MapObj::IsHit(Player& player)
{
	if (!isOpen_)
	{
		return false;
	}
	if (player.GetAction() == "player_AttackFirst"
		|| player.GetAction() == "player_AttackSecond"
		|| player.GetAction() == "player_AttackThird")
	{
		return false;
	}
	auto gRect = own_->getBoundingBox();
	//auto pRect = player.getBoundingBox();
	Size charSize = player.getContentSize() * player.getScale();
	auto pRect = cocos2d::Rect(player.getPosition() - charSize / 2, charSize);

	if (gRect.intersectsRect(pRect))
	{
		// �Q�[�g��������Ƃ��ɁA�L�[���͏��̏�����
		player.KeyInputClear();
		return true;
	}
	return false;
}