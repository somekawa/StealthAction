#pragma once
#include <type_traits>

enum class ActorType
{
	Player,
	Imp,
	Assassin,
	TwistedCultist,
	Cultist,
	Max
};
static ActorType begin(ActorType)
{
	return ActorType::Player;
}
static ActorType end(ActorType)
{
	return ActorType::Max;
}
static ActorType operator++(ActorType& type)
{
	return (type = (ActorType)(std::underlying_type<ActorType>::type(type) + 1));
}
static ActorType operator+(ActorType type,int i)
{
	return static_cast<ActorType>(static_cast<int> (type) + i);
}
static ActorType operator-(ActorType type, int i)
{
	return static_cast<ActorType>(static_cast<int>(type) - i);
}

static ActorType& operator*(ActorType& type)
{
	return type;
}
