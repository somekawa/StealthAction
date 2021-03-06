#include "PhysicalAttackJudgement.h"
#include "obj/Enemy/Enemy.h"

bool PhysicalAttackJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
	// まず距離を見る
	auto distance = dynamic_cast<Enemy&>(sprite).GetDistance();
	// 距離が攻撃範囲内だったら
	if (distance <= dynamic_cast<Enemy&>(sprite).GetVisionRange().attack_ - 10.0f)
	{
		// 攻撃ｱｸｼｮﾝの実行
		return true;
	}
	// それ以外であれば攻撃しない
	// 攻撃ﾌﾗｸﾞをfalseに
	dynamic_cast<Enemy&>(sprite).SetIsAttacking(false);
	return false;
}
