#include "Gravity.h"
#include "obj/Actor.h"

USING_NS_CC;

Gravity::Gravity(Actor& target):
	target_(target)
{
	g_ = 0.0f;
}

Gravity::~Gravity()
{
}

void Gravity::ApplyGravityToTarget(float delta)
{
	// まずターゲットのポジション取得
	auto pos = target_.getPosition();
	if (!target_.OnFloor())
	{
		// 毎回重力に力を加えていく(今のところ1200でテスト)
		g_ += (Gravitation + (delta * 50));
	}
	else
	{
		g_ = 0.0f;
	}

	// ターゲットに重力をかける
	pos.y -= g_;
	// 重力をかけた後の変更後のポジションでターゲットのポジションセット
	//target_.SetPos(pos);
	target_.setPosition(Vec2(pos.x, pos.y));
	//target_.runAction(MoveTo::create(0.0f, Vec2(pos.x, pos.y - g_)));
}
