#pragma once
#include "obj/Actor.h"

class Player;

class Enemy :
	public Actor
{
public:
	Enemy(cocos2d::Vector<cocos2d::Node*>& player_);
	~Enemy();

	void Action(void);
	virtual void ChangeDirection(void);

	virtual const float& DistanceCalcurator(void);
	virtual void update(float delta) {};
	virtual void AnimRegistrator(void) = 0;
	//virtual Enemy* CreateEnemy(std::vector<std::shared_ptr<Player>>& player_) = 0;

	//CREATE_FUNC(Enemy);
private:

protected:
	cocos2d::Vector<cocos2d::Node*>& player_;
};