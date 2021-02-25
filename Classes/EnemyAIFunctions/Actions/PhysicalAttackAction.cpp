#include "PhysicalAttackAction.h"
#include "obj/Enemy/Enemy.h"

bool PhysicalAttackAction::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    // ｱｸｼｮﾝｱﾆﾒｰｼｮﾝのｾｯﾄ
    dynamic_cast<Enemy&>(sprite).SetAction(module.actName);

    auto attackFlg = dynamic_cast<Enemy&>(sprite).IsAttacking();

    if (!attackFlg)
    {
		if (dynamic_cast<Enemy&>(sprite).GetType() == ActorType::Cultist)
		{
			if (!dynamic_cast<Enemy&>(sprite).AddAttackObj(0.0f))
			{
				return true;
			}
		}
		else
		{
			// direction_ == Left
			if (dynamic_cast<Enemy&>(sprite).GetDirection() == Direction::Left)
			{
				// 攻撃矩形に加えるｵﾌｾｯﾄ情報をｾｯﾄ
				// 左向きはﾃﾞﾌｫなのでｵﾌｾｯﾄは加える必要なしなので0
				dynamic_cast<Enemy&>(sprite).SetAttackOffset(cocos2d::Vec2(0.0f, 0.0f));
			}
			// direction_ == Right
			else if (dynamic_cast<Enemy&>(sprite).GetDirection() == Direction::Right)
			{
				if (dynamic_cast<Enemy&>(sprite).GetType() == ActorType::Assassin)
				{
					// 攻撃矩形に加えるｵﾌｾｯﾄ情報をｾｯﾄ
					// 右向きになるとｵﾌｾｯﾄ分ずらす必要があるので、ｵﾌｾｯﾄを65に
					dynamic_cast<Enemy&>(sprite).SetAttackOffset(cocos2d::Vec2(65.0f, 0.0f));
				}
				if (dynamic_cast<Enemy&>(sprite).GetType() == ActorType::TwistedCultist)
				{
					dynamic_cast<Enemy&>(sprite).SetAttackOffset(cocos2d::Vec2(30.0f, 0.0f));
				}
			}
			dynamic_cast<Enemy&>(sprite).SetIsAttacking(true);
		}
		// enemyの子供であるｺﾗｲﾀﾞｰﾎﾞｯｸｽで回す
		for (auto col : dynamic_cast<Enemy&>(sprite).getChildren())
		{
			// 矩形の名前がattackならば
			if (col->getName() == dynamic_cast<Enemy&>(sprite).GetAction())
			{
				// 矩形のﾀｲﾌﾟがattackのやつのみの抽出
				if (col->getLocalZOrder() == 0)
				{
					// 矩形のﾎﾟｼﾞｼｮﾝの変更
					// 矩形のﾎﾟｼﾞｼｮﾝは1度のみの変更でないと
					// 毎ﾌﾚｰﾑ回ってしまうと矩形がどっかいくのでここでｾｯﾄしている。
					col->setPosition(dynamic_cast<Enemy&>(sprite).GetAttackRect().offset_.x, col->getPosition().y);
				}
			}
		}
    }
	dynamic_cast<Enemy&>(sprite).NormalAttack();
	return true;

}
