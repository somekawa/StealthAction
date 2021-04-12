#include "PhysicalAttackJudgement.h"
#include "obj/Enemy/Enemy.h"

// trueを返せばこのActionを実行する
bool PhysicalAttackJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
	// 距離が攻撃範囲内だったら
	if (dynamic_cast<Enemy&>(sprite).GetDistance() <= dynamic_cast<Enemy&>(sprite).GetVisionRange().attack_ - 10.0f)
	{
		// 攻撃アクションの実行
		return true;
	}
	// それ以外であれば攻撃しない
	// 攻撃フラグをfalseに
	dynamic_cast<Enemy&>(sprite).SetIsAttacking(false);
	return false;
}