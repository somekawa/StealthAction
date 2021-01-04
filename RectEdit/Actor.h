#pragma once
#include <type_traits>

// ÌßÚ²Ô°or´ÈÐ°‚Ì‹æ•Ê‚·‚é‚½‚ß‚Ìenum¸×½
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
