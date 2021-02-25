#include "MapObj.h"
#include "../obj/Player.h"

USING_NS_CC;

MapObj::MapObj()
{
}

MapObj::~MapObj()
{
}

void MapObj::Update(int nowEnemyNum)
{
	if (nowEnemyNum == 0)
	{
		isEnable_ = true;
		this->setVisible(isEnable_);
	}
}

bool MapObj::IsHit(Player& player)
{
	if (!isEnable_)
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
		// ゲートをくぐるときに、キー入力情報の初期化
		player.KeyInputClear();
		return true;
	}
	return false;
}
