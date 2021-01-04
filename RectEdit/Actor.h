#pragma once
#include <type_traits>

// ��ڲ԰or��а�̋�ʂ��邽�߂�enum�׽
enum class Actor
{
	Player,
	Enemy,
	Max
};

static Actor begin(Actor)
{
	return Actor::Player;
}

static Actor end(Actor)
{
	return Actor::Max;
}

static Actor operator++(Actor& actor)
{
	return (actor = (Actor)(std::underlying_type<Actor>::type(actor) + 1));
}

static Actor operator*(Actor& actor)
{
	return actor;
}

static Actor operator+(Actor& actor,int i)
{
	return static_cast<Actor>(static_cast<int>(actor) + i);
}
