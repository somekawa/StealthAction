#pragma once
#include <type_traits>

// �A�N�^�[�̎��
enum class ActorType
{
	Player,
	Assassin,
	TwistedCultist,
	Cultist,
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