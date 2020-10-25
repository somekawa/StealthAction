#include "Actor.h"
#include "GameScene.h"

//Actor::Actor()
//{
//}

Actor::Actor(std::unordered_map<std::string, std::vector<std::vector<std::shared_ptr<ActionRect>>>>& collider):
	collider_(collider)
{
	deleteFlag_ = false;
	animationFrame_ = 0.0f;
}

Actor::~Actor()
{
}

void Actor::Update(void)
{
	Action();
}

