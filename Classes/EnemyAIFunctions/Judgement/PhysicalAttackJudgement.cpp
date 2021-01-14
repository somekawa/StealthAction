#include "PhysicalAttackJudgement.h"
#include "obj/Enemy/Enemy.h"

bool PhysicalAttackJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
	// �܂�����������
	auto distance = dynamic_cast<Enemy&>(sprite).GetDistance();
	// �������U���͈͓���������
	if (distance <= dynamic_cast<Enemy&>(sprite).GetVisionRange().attack_ - 10.0f)
	{
		// �U������݂̎��s
		return true;
	}
	// ����ȊO�ł���΍U�����Ȃ�
	// �U���׸ނ�false��
	dynamic_cast<Enemy&>(sprite).SetIsAttacking(false);
	return false;
}
