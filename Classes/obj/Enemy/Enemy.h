#pragma once
#include "obj/Actor.h"


class Player;

class Enemy :
	public Actor
{
public:
	Enemy(std::vector<std::shared_ptr<Player>>& player_);
	~Enemy();

	void Action(void);
	virtual void ChangeDirection(void);

	virtual const float& DistanceCalcurator(void);
	static Enemy* CreateEnemy(std::vector<std::shared_ptr<Player>>& player_);

	//CREATE_FUNC(Enemy);
private:

protected:
	std::vector<std::shared_ptr<Player>>& player_;
};