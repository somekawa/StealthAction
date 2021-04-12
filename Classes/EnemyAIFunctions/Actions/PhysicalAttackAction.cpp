#include "PhysicalAttackAction.h"
#include "obj/Enemy/Enemy.h"

bool PhysicalAttackAction::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
    // �A�N�V�����A�j���[�V�����̃Z�b�g
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
		// �U����`�ɉ�����I�t�Z�b�g�����Z�b�g
		if (dynamic_cast<Enemy&>(sprite).GetDirection() == Direction::Left)
		{
			// �������̓f�t�H���g������A�I�t�Z�b�g�l��0
			dynamic_cast<Enemy&>(sprite).SetAttackOffset(cocos2d::Vec2(0.0f, 0.0f));
		}
		else if (dynamic_cast<Enemy&>(sprite).GetDirection() == Direction::Right)
		{
			// �E�����ɂȂ�ƃI�t�Z�b�g���K�v������A�I�t�Z�b�g�l���Z�b�g
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
			// �����������s��Ȃ�
		}
		dynamic_cast<Enemy&>(sprite).SetIsAttacking(true);
	}

	// enemy�̎q���ł���R���C�_�[�{�b�N�X�ŉ�
	for (auto col : dynamic_cast<Enemy&>(sprite).getChildren())
	{
		if (col->getName() != dynamic_cast<Enemy&>(sprite).GetAction())
		{
			// ��`�̖��O��attack�ȊO�Ȃ�Ή����s��Ȃ�
			continue;
		}

		// ��`�̃^�C�v��attack�̂�݂̂̒��o
		if (col->getLocalZOrder() == 0)
		{
			// ��`�̃|�W�V�����̕ύX
			col->setPosition(dynamic_cast<Enemy&>(sprite).GetAttackRect().offset_.x, col->getPosition().y);
		}
	}
	dynamic_cast<Enemy&>(sprite).NormalAttack();
	return true;
}