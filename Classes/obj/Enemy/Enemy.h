#pragma once
#include "obj/Actor.h"

class Player;

class Enemy :
	public Actor
{
public:
	Enemy(cocos2d::Vector<cocos2d::Node*>& player_, std::unordered_map<std::string,
		std::vector<std::vector<std::shared_ptr<ActionRect>>>>&collider);
	~Enemy();

	void Action(void);
	virtual void ChangeDirection(void);

	virtual const float& DistanceCalcurator(void);
	virtual void update(float delta) {};

	//virtual Enemy* CreateEnemy(std::vector<std::shared_ptr<Player>>& player_) = 0;

	//CREATE_FUNC(Enemy);
private:

protected:
	cocos2d::Vector<cocos2d::Node*>& player_;
};