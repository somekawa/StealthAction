#pragma once
#include <type_traits>

// アクターの種類
enum class ActorType
{
	Player,
	Assassin,
	TwistedCultist,
	Cultist,
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