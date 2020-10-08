#include "CheckObjHit.h"

USING_NS_CC;

bool CheckObjHit::operator()(cocos2d::Sprite & sprite, ActModule & module)
{
	auto director = Director::getInstance();
	if ((TMXLayer*)director->getRunningScene()->getChildByName("BG_BACK")->getChildByName("MapData") == nullptr)
	{
		return false;
	}
	const int chipSize = 48;
	auto plPos = sprite.getPosition();
	auto CollisionData = (TMXLayer*)director->getRunningScene()->getChildByName("BG_BACK")->getChildByName("MapData")->getChildByName("col");
	auto ColSize = CollisionData->getLayerSize();
	auto plCheckPoint1 = plPos + module.checkPoint1;
	auto plCheckPoint2 = plPos + module.checkPoint2;

	

	auto plCheckPoint1Chip = Vec2{ plCheckPoint1 } / chipSize;
	auto plCheckPoint2Chip = Vec2{ plCheckPoint2 } / chipSize;

	auto plCheckPoint1Pos = Vec2(plCheckPoint1Chip.x, ColSize.height - plCheckPoint1Chip.y);
	auto plCheckPoint2Pos = Vec2(plCheckPoint2Chip.x, ColSize.height - plCheckPoint2Chip.y);

	// �͈͊Ocheck
	if (plCheckPoint1Pos.x > ColSize.width || plCheckPoint1Pos.x < 0 ||
		plCheckPoint1Pos.y > ColSize.height || plCheckPoint1Pos.y < 0)
	{
		return false;
	}


	auto plCheckPoint1Gid = CollisionData->getTileGIDAt(plCheckPoint1Pos);
	auto plCheckPoint2Gid = CollisionData->getTileGIDAt(plCheckPoint2Pos);

	if (plCheckPoint1Gid != 0 || plCheckPoint2Gid != 0)
	{
		//sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - (module.flame));
		return false;
	}

	return true;
}
