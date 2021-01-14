#include "CheckObjHit.h"
#include "scene/GameScene.h"
#include "obj/Player.h"
#include "obj/Enemy/Enemy.h"

USING_NS_CC;

bool CheckObjHit::operator()(cocos2d::Sprite & sprite, ActModule & module)
{
	auto director = Director::getInstance();
	if ((TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData") == nullptr)
	{
		return false;
	}

	auto plPos = sprite.getPosition();
	auto CollisionData = (TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData")->getChildByName("col");
	auto WallSlideData = (TMXLayer*)director->getRunningScene()->getChildByTag((int)zOlder::BG)->getChildByName("MapData")->getChildByName("slide");
	auto& ColSize = CollisionData->getLayerSize();
	const int chipSize = CollisionData->getMapTileSize().width;
	// 当たり判定の位置に+module.velをすることで、次回移動値を加算したpointで当たり判定をするようにした
	// →カメラのブレをなくすため
	auto plCheckPoint1 = plPos + module.checkPoint1 + module.vel;
	auto plCheckPoint2 = plPos + module.checkPoint2 + module.vel;
	

	auto plCheckPoint1Chip = Vec2{ plCheckPoint1 } / chipSize;
	auto plCheckPoint2Chip = Vec2{ plCheckPoint2 } / chipSize;

	auto plCheckPoint1Pos = Vec2(plCheckPoint1Chip.x, ColSize.height - plCheckPoint1Chip.y);
	auto plCheckPoint2Pos = Vec2(plCheckPoint2Chip.x, ColSize.height - plCheckPoint2Chip.y);

	// 範囲外check
	if (plCheckPoint1Pos.x > ColSize.width || plCheckPoint1Pos.x < 0 ||
		plCheckPoint1Pos.y > ColSize.height || plCheckPoint1Pos.y < 0)
	{
		return false;
	}


	auto plCheckPoint1Gid = CollisionData->getTileGIDAt(plCheckPoint1Pos);
	auto plCheckPoint2Gid = CollisionData->getTileGIDAt(plCheckPoint2Pos);

	auto plCheckPoint1GidW = WallSlideData->getTileGIDAt(plCheckPoint1Pos);
	auto plCheckPoint2GidW = WallSlideData->getTileGIDAt(plCheckPoint2Pos);

	auto lambda = [&](Vec2 point) {
		auto plCheckPointChip = Vec2{ point } / chipSize;
		auto plCheckPointPos = Vec2(plCheckPointChip.x, ColSize.height - plCheckPointChip.y);
		auto plCheckPointGid = CollisionData->getTileGIDAt(plCheckPointPos);
		if (plCheckPointGid != 0)
		{
			// 補正が必要なときにtrue
			return true;
		}
		else
		{
			return false;
		}
		return false;
	};

	if ((plCheckPoint1GidW != 0 && plCheckPoint2GidW != 0))
	{
		((Player&)sprite).SetAction("player_Wall_Slide");
		return false;
	}

	// 段差落下時の補正処理
	if (module.actName == "Fall")
	{
		if (lambda(plPos + module.checkPoint3))
		{
			sprite.setPosition(sprite.getPosition().x - 1.0f, sprite.getPosition().y);
		}
		else if (lambda(plPos + module.checkPoint4))
		{
			sprite.setPosition(sprite.getPosition().x + 1.0f, sprite.getPosition().y);
		}
	}

	if (plCheckPoint1Gid != 0 || plCheckPoint2Gid != 0)
	{
		//sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - (module.flame));
		return false;
	}

	return true;
}
