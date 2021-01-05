#pragma once
#include <type_traits>

enum class ActorType
{
	Player,
	Imp,
	Assassin,
	TwistedCultist,
	Cultist,
	BigCultist,
	Fireball,
	Max
};

// InputID��begin�̒�`
static ActorType begin(ActorType)
{
	return ActorType::Player;
}
// InputID��end�̒�`
static ActorType end(ActorType)
{
	return ActorType::Max;
}
// InputID++�̒�`
static ActorType operator++(ActorType& id)
{
	return (id = ActorType(std::underlying_type<ActorType>::type(id) + 1));
}
// �������g��Ԃ��֐�
static ActorType operator*(ActorType id)
{
	return id;
}