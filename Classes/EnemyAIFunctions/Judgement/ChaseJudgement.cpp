#include "ChaseJudgement.h"
#include "obj/Enemy/Enemy.h"

// true��Ԃ��΂���Action������
bool ChaseJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
	if (dynamic_cast<Enemy&>(sprite).GetDistance() <= 100.0f)
	{
		
		//return true;
	}
	return false;
}
