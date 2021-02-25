#include "PhysicalAttackAction.h"
#include "obj/Enemy/Enemy.h"

bool PhysicalAttackAction::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    // ����ݱ�Ұ��݂̾��
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
				// �U����`�ɉ�����̾�ď����
				// ����������̫�Ȃ̂ŵ̾�Ă͉�����K�v�Ȃ��Ȃ̂�0
				dynamic_cast<Enemy&>(sprite).SetAttackOffset(cocos2d::Vec2(0.0f, 0.0f));
			}
			// direction_ == Right
			else if (dynamic_cast<Enemy&>(sprite).GetDirection() == Direction::Right)
			{
				if (dynamic_cast<Enemy&>(sprite).GetType() == ActorType::Assassin)
				{
					// �U����`�ɉ�����̾�ď����
					// �E�����ɂȂ�Ƶ̾�ĕ����炷�K�v������̂ŁA�̾�Ă�65��
					dynamic_cast<Enemy&>(sprite).SetAttackOffset(cocos2d::Vec2(65.0f, 0.0f));
				}
				if (dynamic_cast<Enemy&>(sprite).GetType() == ActorType::TwistedCultist)
				{
					dynamic_cast<Enemy&>(sprite).SetAttackOffset(cocos2d::Vec2(30.0f, 0.0f));
				}
			}
			dynamic_cast<Enemy&>(sprite).SetIsAttacking(true);
		}
		// enemy�̎q���ł���ײ�ް�ޯ���ŉ�
		for (auto col : dynamic_cast<Enemy&>(sprite).getChildren())
		{
			// ��`�̖��O��attack�Ȃ��
			if (col->getName() == dynamic_cast<Enemy&>(sprite).GetAction())
			{
				// ��`�����߂�attack�̂�݂̂̒��o
				if (col->getLocalZOrder() == 0)
				{
					// ��`���߼޼�݂̕ύX
					// ��`���߼޼�݂�1�x�݂̂̕ύX�łȂ���
					// ���ڰщ���Ă��܂��Ƌ�`���ǂ��������̂ł����ž�Ă��Ă���B
					col->setPosition(dynamic_cast<Enemy&>(sprite).GetAttackRect().offset_.x, col->getPosition().y);
				}
			}
		}
    }
	dynamic_cast<Enemy&>(sprite).NormalAttack();
	return true;

}
