#include "BehaviorBaseAI/JudgementTool/AttackJudgement.h"
#include "obj/Enemy/Enemy.h"

bool AttackJudgement::Judgement(Enemy* enemy)
{
	if (enemy->DistanceCalcurator() < enemy->GetVisionRange().attack_)
	{
		if (!enemy->IsAttacking())
		{
			if (enemy->GetType() == ActorType::Cultist)
			{
				enemy->AddAttackObj();
			}
			// direction_ == Left
			if (enemy->GetDirection() == Direction::Left)
			{
				// 攻撃矩形に加えるｵﾌｾｯﾄ情報をｾｯﾄ
				// 左向きはﾃﾞﾌｫなのでｵﾌｾｯﾄは加える必要なしなので0
				enemy->SetAttackOffset(cocos2d::Vec2(0.0f, 0.0f));
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
					enemy->SetAttackOffset(cocos2d::Vec2(30.0f,0.0f));
				}
			}
			// ｱﾆﾒｰｼｮﾝの変更
			enemy->ChangeAnimation("attack");
			// enemyの子供であるｺﾗｲﾀﾞｰﾎﾞｯｸｽで回す
			for (auto col:enemy->getChildren())
			{
				// 矩形の名前がattackならば
				if (col->getName() == enemy->GetAction())
				{
					// 矩形のﾀｲﾌﾟがattackのやつのみの抽出
					if (col->getLocalZOrder() == 0)
					{
						// 矩形のﾎﾟｼﾞｼｮﾝの変更
						// 矩形のﾎﾟｼﾞｼｮﾝは1度のみの変更でないと
						// 毎ﾌﾚｰﾑ回ってしまうと矩形がどっかいくのでここでｾｯﾄしている。
						col->setPosition(enemy->GetAttackRect().offset_.x, col->getPosition().y);
					}
				}
			}
			enemy->SetIsAttacking(true);
		}
		return true;
	}
	return false;
}
