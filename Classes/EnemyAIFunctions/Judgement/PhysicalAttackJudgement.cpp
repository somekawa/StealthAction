#include "PhysicalAttackJudgement.h"
#include "obj/Enemy/Enemy.h"

// true��Ԃ��΂���Action�����s����
bool PhysicalAttackJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
	// �������U���͈͓���������
	if (dynamic_cast<Enemy&>(sprite).GetDistance() <= dynamic_cast<Enemy&>(sprite).GetVisionRange().attack_ - 10.0f)
	{
		// �U���A�N�V�����̎��s
		return true;
	}
	// ����ȊO�ł���΍U�����Ȃ�
	// �U���t���O��false��
	dynamic_cast<Enemy&>(sprite).SetIsAttacking(false);
	return false;
}