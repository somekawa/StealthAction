#include "Actor.h"
#include "Scene/GameScene.h"

//Actor::Actor()
//{
//}

Actor::Actor()
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

