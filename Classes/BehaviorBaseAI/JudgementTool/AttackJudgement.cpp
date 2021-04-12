#include "BehaviorBaseAI/JudgementTool/AttackJudgement.h"
#include "obj/Enemy/Enemy.h"
#include "obj/Objects/Fireball.h"
#include "obj/Enemy/Cultist.h"

bool AttackJudgement::Judgement(Enemy* enemy)
{
	if (enemy->GetDistance() >= enemy->GetVisionRange().attack_)
	{
		return false;
	}

	if (enemy->IsAttacking())
	{
		return true;
	}

	if (enemy->GetType() == ActorType::Cultist)
	{
		if (!enemy->GetFire())
		{
			auto angle = dynamic_cast<Cultist&>(*enemy).GetPLAngle();
			enemy->AddAttackObj(angle);
			enemy->SetIsAttacking(true);
		}
	}
	else
	{
		// direction_ == Left
		if (enemy->GetDirection() == Direction::Left)
		{
			// 攻撃矩形に加えるｵﾌｾｯﾄ情報をｾｯﾄ
			// 左向きはﾃﾞﾌｫなのでｵﾌｾｯﾄは加える必要なしなので0
			enemy->SetAttackOffset(cocos2d::Vec2(0.0f, 0.0f));
			enemy->SetIsAttacking(true);
		}
		// direction_ == Right
		else if (enemy->GetDirection() == Direction::Right)
		{
			if (enemy->GetType() == ActorType::Assassin)
			{
				// 攻撃矩形に加えるｵﾌｾｯﾄ情報をｾｯﾄ
				// 右向きになるとｵﾌｾｯﾄ分ずらす必要があるので、ｵﾌｾｯﾄを65に
				enemy->SetAttackOffset(cocos2d::Vec2(65.0f, 0.0f));
			}
			if (enemy->GetType() == ActorType::TwistedCultist)
			{
				enemy->SetAttackOffset(cocos2d::Vec2(30.0f, 0.0f));
			}
			enemy->SetIsAttacking(true);
		}
		else
		{
			// 何も処理を行わない
		}
	}

	// ｱﾆﾒｰｼｮﾝの変更
	enemy->ChangeAnimation("attack");
	// enemyの子供であるｺﾗｲﾀﾞｰﾎﾞｯｸｽで回す
	for (auto col:enemy->getChildren())
	{
		// 矩形の名前がattack以外ならば処理を行わない
		if (col->getName() != enemy->GetAction())
		{
			continue;
		}

		// 矩形のﾀｲﾌﾟがattackのやつのみの抽出
		if (col->getLocalZOrder() == 0)
		{
			// 矩形のﾎﾟｼﾞｼｮﾝの変更
			// 矩形のﾎﾟｼﾞｼｮﾝは1度のみの変更でないと
			// 毎ﾌﾚｰﾑ回ってしまうと矩形がどっかいくのでここでｾｯﾄしている。
			col->setPosition(enemy->GetAttackRect().offset_.x, col->getPosition().y);
		}
	}
	return false;
}