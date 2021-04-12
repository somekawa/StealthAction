// �L�[�����Ɋ֘A���鏈���̎�����S��
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

	if (player.GetAction() == "player_AttackFirst" ||
		player.GetAction() == "player_AttackSecond"||
		player.GetAction() == "player_AttackThird")
	{
		return false;
	}

	const auto gRect = own_->getBoundingBox();
	const Size charSize = player.getContentSize() * player.getScale();

	if (gRect.intersectsRect(cocos2d::Rect(player.getPosition() - charSize / 2.0f, charSize)))
	{
		// �Q�[�g��������Ƃ��ɁA�L�[���͏��̏�����
		player.KeyInputClear();
		return true;
	}
	return false;
}