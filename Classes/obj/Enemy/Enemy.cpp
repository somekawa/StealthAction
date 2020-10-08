#include "obj/Enemy/Enemy.h"
#include "obj/Player.h"

USING_NS_CC;

Enemy::Enemy(std::vector<std::shared_ptr<Player>>& player):
	player_(player)
{

}

Enemy::~Enemy()
{
}


void Enemy::Action(void)
{

}

void Enemy::ChangeDirection(void)
{
	auto playerPos = player_[0]->GetPos();

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
	auto playerPos = player_[0]->GetPos();

	return abs(playerPos.x - pos_.x);
}

Enemy* Enemy::CreateEnemy(std::vector<std::shared_ptr<Player>>& player_)
{

	Enemy* pRet = new(std::nothrow) Enemy(player_);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}
