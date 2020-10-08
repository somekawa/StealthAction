#include "Actor.h"

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

