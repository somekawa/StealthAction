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
				// �U����`�ɉ�����̾�ď����
				// ����������̫�Ȃ̂ŵ̾�Ă͉�����K�v�Ȃ��Ȃ̂�0
				enemy->SetAttackOffset(cocos2d::Vec2(0.0f, 0.0f));
			}
			// direction_ == Right
			else if (enemy->GetDirection() == Direction::Right)
			{
				if (enemy->GetType() == ActorType::Assassin)
				{
					// �U����`�ɉ�����̾�ď����
					// �E�����ɂȂ�Ƶ̾�ĕ����炷�K�v������̂ŁA�̾�Ă�65��
					enemy->SetAttackOffset(cocos2d::Vec2(65.0f, 0.0f));
				}
				if (enemy->GetType() == ActorType::TwistedCultist)
				{
					enemy->SetAttackOffset(cocos2d::Vec2(30.0f,0.0f));
				}
			}
			// ��Ұ��݂̕ύX
			enemy->ChangeAnimation("attack");
			// enemy�̎q���ł���ײ�ް�ޯ���ŉ�
			for (auto col:enemy->getChildren())
			{
				// ��`�̖��O��attack�Ȃ��
				if (col->getName() == enemy->GetAction())
				{
					// ��`�����߂�attack�̂�݂̂̒��o
					if (col->getLocalZOrder() == 0)
					{
						// ��`���߼޼�݂̕ύX
						// ��`���߼޼�݂�1�x�݂̂̕ύX�łȂ���
						// ���ڰщ���Ă��܂��Ƌ�`���ǂ��������̂ł����ž�Ă��Ă���B
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
