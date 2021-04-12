// プレイヤー関連担当
#include "scene/GameScene.h"
#include "obj/Player.h"
#include "obj/Enemy/Enemy.h"
#include "obj/Enemy/Assassin.h"
#include "CheckObjHit.h"

USING_NS_CC;

bool CheckObjHit::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
	if (static_cast<TMXLayer*>(Director::getInstance()->getRunningScene()->getChildByTag(static_cast<int>(zOrder::BG))->getChildByName("MapData")) == nullptr)
	{
		return false;
	}

	const Vec2 plPos = sprite.getPosition();
	TMXLayer* collisionData = static_cast<TMXLayer*>(Director::getInstance()->getRunningScene()->getChildByTag(static_cast<int>(zOrder::BG))->getChildByName("MapData")->getChildByName("col"));
	TMXLayer* wallSlideData = static_cast<TMXLayer*>(Director::getInstance()->getRunningScene()->getChildByTag(static_cast<int>(zOrder::BG))->getChildByName("MapData")->getChildByName("slide"));
	const Size& colSize  = collisionData->getLayerSize();
	const float chipSize = collisionData->getMapTileSize().width;

	const Vec2 plCheckPoint1 = plPos + module.checkPoint1 + module.vel;
	const Vec2 plCheckPoint2 = plPos + module.checkPoint2 + module.vel;

	const Vec2 plCheckPoint1Chip = plCheckPoint1 / chipSize;
	const Vec2 plCheckPoint2Chip = plCheckPoint2 / chipSize;

	// Vec2 test = Vec2(xxx,yyy)と書かないといけないぐらいなら、Vec2 test{xxx,yyy}のほうが冗長にならない
	const Vec2 plCheckPoint1Pos{ plCheckPoint1Chip.x, colSize.height - plCheckPoint1Chip.y };
	const Vec2 plCheckPoint2Pos{ plCheckPoint2Chip.x, colSize.height - plCheckPoint2Chip.y };

	// 範囲外check
	if (plCheckPoint1Pos.x >= colSize.width  || plCheckPoint1Pos.x <= 0 ||
		plCheckPoint1Pos.y >= colSize.height || plCheckPoint1Pos.y <= 0)
	{
		return false;
	}
	if (plCheckPoint2Pos.x >= colSize.width  || plCheckPoint2Pos.x <= 0 ||
		plCheckPoint2Pos.y >= colSize.height || plCheckPoint2Pos.y <= 0)
	{
		return false;
	}

	const auto plCheckPoint1Gid = collisionData->getTileGIDAt(plCheckPoint1Pos);
	const auto plCheckPoint2Gid = collisionData->getTileGIDAt(plCheckPoint2Pos);

	const auto plCheckPoint1GidW = wallSlideData->getTileGIDAt(plCheckPoint1Pos);
	const auto plCheckPoint2GidW = wallSlideData->getTileGIDAt(plCheckPoint2Pos);

	// 座標補正が必要かをチェックする
	auto correctionLambda = [&](Vec2 point) {
		const Vec2 plCheckPointChip = point / chipSize;
		const auto plCheckPointGid = collisionData->getTileGIDAt({ plCheckPointChip.x, colSize.height - plCheckPointChip.y });
		if (plCheckPointGid != 0)
		{
			// 補正が必要なときにtrue
			return true;
		}
		return false;
	};

	if (sprite.getName() == "player1")
	{
		if ((plCheckPoint1GidW != 0 && plCheckPoint2GidW != 0))
		{
			dynamic_cast<Player&>(sprite).SetAction("Wall_Slide");
			return false;
		}
	}
	else
	{
		const Direction direction = dynamic_cast<Enemy&>(sprite).GetDirection();
		if (direction == Direction::Right)
		{
			if ((dynamic_cast<Enemy&>(sprite).IsMove(direction)) && module.commonAction == AnimationType::R_move)
			{
				if (plCheckPoint1GidW != 0 && plCheckPoint2GidW != 0)
				{
					dynamic_cast<Enemy&>(sprite).SetMove(direction, false);
					return false;
				}
			}
		}
		else
		{
			if ((dynamic_cast<Enemy&>(sprite).IsMove(direction)) && module.commonAction == AnimationType::L_move)
			{
				if (plCheckPoint1GidW != 0 && plCheckPoint2GidW != 0)
				{
					dynamic_cast<Enemy&>(sprite).SetMove(direction, false);
					return false;
				}
			}
		}
	}

	// 段差落下時の補正処理
	if (module.actName == "Fall")
	{
		if (correctionLambda(plPos + module.checkPoint3))
		{
			sprite.setPosition(sprite.getPosition().x - 1.0f, sprite.getPosition().y);
		}
		else if (correctionLambda(plPos + module.checkPoint4))
		{
			sprite.setPosition(sprite.getPosition().x + 1.0f, sprite.getPosition().y);
		}
		else
		{
			// 何も処理を行わない
		}
	}

	// オブジェクトと当たった時にfalseを返す
	if (plCheckPoint1Gid != 0 || plCheckPoint2Gid != 0)
	{
		return false;
	}

	return true;
}