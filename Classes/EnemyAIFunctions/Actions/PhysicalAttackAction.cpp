#include "PhysicalAttackAction.h"
#include "obj/Enemy/Enemy.h"

bool PhysicalAttackAction::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    // アクションアニメーションのセット
    dynamic_cast<Enemy&>(sprite).SetAction(module.actName);

	if (dynamic_cast<Enemy&>(sprite).IsAttacking())
	{
		dynamic_cast<Enemy&>(sprite).NormalAttack();
		return true;
	}

	if (dynamic_cast<Enemy&>(sprite).GetType() == ActorType::Cultist)
	{
		if (!dynamic_cast<Enemy&>(sprite).AddAttackObj(0.0f))
		{
			return true;
		}
	}
	else
	{
		// 攻撃矩形に加えるオフセット情報をセット
		if (dynamic_cast<Enemy&>(sprite).GetDirection() == Direction::Left)
		{
			// 左向きはデフォルトだから、オフセット値は0
			dynamic_cast<Enemy&>(sprite).SetAttackOffset(cocos2d::Vec2(0.0f, 0.0f));
		}
		else if (dynamic_cast<Enemy&>(sprite).GetDirection() == Direction::Right)
		{
			// 右向きになるとオフセットが必要だから、オフセット値をセット
			if (dynamic_cast<Enemy&>(sprite).GetType() == ActorType::Assassin)
			{
				dynamic_cast<Enemy&>(sprite).SetAttackOffset(cocos2d::Vec2(65.0f, 0.0f));
			}
			if (dynamic_cast<Enemy&>(sprite).GetType() == ActorType::TwistedCultist)
			{
				dynamic_cast<Enemy&>(sprite).SetAttackOffset(cocos2d::Vec2(30.0f, 0.0f));
			}
		}
		else
		{
			// 何も処理を行わない
		}
		dynamic_cast<Enemy&>(sprite).SetIsAttacking(true);
	}

	// enemyの子供であるコライダーボックスで回す
	for (auto col : dynamic_cast<Enemy&>(sprite).getChildren())
	{
		if (col->getName() != dynamic_cast<Enemy&>(sprite).GetAction())
		{
			// 矩形の名前がattack以外ならば何も行わない
			continue;
		}

		// 矩形のタイプがattackのやつのみの抽出
		if (col->getLocalZOrder() == 0)
		{
			// 矩形のポジションの変更
			col->setPosition(dynamic_cast<Enemy&>(sprite).GetAttackRect().offset_.x, col->getPosition().y);
		}
	}
	dynamic_cast<Enemy&>(sprite).NormalAttack();
	return true;
}