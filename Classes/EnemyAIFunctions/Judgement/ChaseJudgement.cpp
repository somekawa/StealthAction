#include "ChaseJudgement.h"
#include "obj/Enemy/Enemy.h"

// trueを返せばこのActionをする
bool ChaseJudgement::operator()(cocos2d::Sprite& sprite, ActModule& module)
{
	if (dynamic_cast<Enemy&>(sprite).GetDistance() <= 100.0f)
	{
		
		//return true;
	}
	return false;
}
