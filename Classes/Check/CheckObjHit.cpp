#include "CheckObjHit.h"

USING_NS_CC;

bool CheckObjHit::operator()(cocos2d::Sprite & sprite, ActModule & module)
{
	auto director = Director::getInstance();
	auto a = director->getRunningScene()->getChildByName("BG_BACK")->getChildByName("MapData");
	if ((TMXLayer*)director->getRunningScene()->getChildByName("BG_BACK")->getChildByName("MapData")->getChildByName("col") == nullptr)
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

	// 範囲外check
	if (plCheckPoint1Pos.x > ColSize.width || plCheckPoint1Pos.x < 0 ||
		plCheckPoint1Pos.y > ColSize.height || plCheckPoint1Pos.y < 0)
	{
		return false;
	}


	auto plCheckPoint1Gid = CollisionData->getTileGIDAt(plCheckPoint1Pos);
	auto plCheckPoint2Gid = CollisionData->getTileGIDAt(plCheckPoint2Pos);

	// 段差落下時の補正処理
	if (module.action == ACTION::FALLING)
	{
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

		if (lambda(plPos + module.checkPoint3))
		{
			sprite.setPosition(sprite.getPosition().x - 1.0f, sprite.getPosition().y);
		}
		else if (lambda(plPos + module.checkPoint4))
		{
			sprite.setPosition(sprite.getPosition().x + 1.0f, sprite.getPosition().y);
		}

		//auto plCheckPoint3 = plPos + module.checkPoint3;
		//auto plCheckPoint3Chip = Vec2{ plCheckPoint3 } / chipSize;
		//auto plCheckPoint3Pos = Vec2(plCheckPoint3Chip.x, ColSize.height - plCheckPoint3Chip.y);
		//auto plCheckPoint3Gid = CollisionData->getTileGIDAt(plCheckPoint3Pos);
		//auto plCheckPoint4 = plPos + module.checkPoint4;
		//auto plCheckPoint4Chip = Vec2{ plCheckPoint4 } / chipSize;
		//auto plCheckPoint4Pos = Vec2(plCheckPoint4Chip.x, ColSize.height - plCheckPoint4Chip.y);
		//auto plCheckPoint4Gid = CollisionData->getTileGIDAt(plCheckPoint4Pos);
		//if (plCheckPoint3Gid != 0)
		//{
		//	sprite.setPosition(sprite.getPosition().x - 1.0f, sprite.getPosition().y);
		//}
		//else if (plCheckPoint4Gid != 0)
		//{
		//	sprite.setPosition(sprite.getPosition().x + 1.0f, sprite.getPosition().y);
		//}
	}

	if (plCheckPoint1Gid != 0 || plCheckPoint2Gid != 0)
	{
		//sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - (module.flame));
		return false;
	}

	return true;
}
