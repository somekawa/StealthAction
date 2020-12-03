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

bool Gravity::ApplyGravityToTarget(float delta)
{
	// まずターゲットのポジション取得
	auto pos = target_.GetPos();
	// 毎回重力に力を加えていく(今のところ1200でテスト)
	g_ += (Gravitation+(delta * 1200));
	// ターゲットに重力をかける
	pos.y -= g_;
	if (pos.y <= 200)
	{
		pos.y = 200;
	}
	// 重力をかけた後の変更後のポジションでターゲットのポジションセット
	target_.SetPos(pos);
	target_.setPosition(Vec2(pos.x, pos.y));
	return true;
}
