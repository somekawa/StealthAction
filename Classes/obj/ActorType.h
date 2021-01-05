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

// InputIDのbeginの定義
static ActorType begin(ActorType)
{
	return ActorType::Player;
}
// InputIDのendの定義
static ActorType end(ActorType)
{
	return ActorType::Max;
}
// InputID++の定義
static ActorType operator++(ActorType& id)
{
	return (id = ActorType(std::underlying_type<ActorType>::type(id) + 1));
}
// 自分自身を返す関数
static ActorType operator*(ActorType id)
{
	return id;
}