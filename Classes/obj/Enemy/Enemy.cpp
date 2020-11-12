#include "obj/Enemy/Enemy.h"
#include "obj/Player.h"

USING_NS_CC;

Enemy::Enemy(Vector<Node*>& player):
	player_(player)
{

}

Enemy::~Enemy()
{
	//onExit();
}


void Enemy::Action(void)
{

}

void Enemy::ChangeDirection(void)
{
	auto playerPos = player_.front()->getPosition();

	if (pos_.x > playerPos.x)
	{
		direction_ = Direction::Left;
	}
	else
	{
		direction_ = Direction::Right;
	}
}

const float& Enemy::DistanceCalcurator(void)
{
	auto playerPos = player_.front()->getPosition();

	return abs(playerPos.x - pos_.x);
}
