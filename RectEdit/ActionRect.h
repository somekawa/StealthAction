// ‹éŒ`î•ñ¸×½
#pragma once
#include <vector>
#include <array>
#include <string>
#include <type_traits>
#include "Geometory.h"


enum class RectType
{
	// UŒ‚‹éŒ`
	Attack,
	// UŒ‚“–‚½‚è‹éŒ`
	Damage,
	Max
};

static RectType begin(RectType)
{
	return RectType::Attack;
}
static RectType end(RectType)
{
	return RectType::Max;
}
static RectType operator++(RectType& type)
{
	return (type = (RectType)(std::underlying_type<RectType>::type(type) + 1));
}
static RectType operator*(RectType& type)
{
	return type;
}

using RectArray = std::array<Vector2I, static_cast<int>(RectType::Max)>;

struct RectData
{
	int frame_;
	Vector2I begin_;
	Vector2I end_;
	Vector2I size_;
	// 0:Attack 1:damage
	char type_;

	RectData() :begin_({ 0,0 }), end_({ 0,0 }),
		size_({ 0,0 }), type_(0), frame_(0) {};

	RectData(Vector2I begin, Vector2I end, Vector2I size, char type, int frame) :
		begin_(begin), end_(end), size_(size), type_(type), frame_(frame) {};
};

class ActionRect
{
public:
	ActionRect(Vector2I begin, Vector2I end, Vector2I offset, char type, int frame);
	~ActionRect();

	void Set(Vector2I begin, Vector2I end, RectType type, int frame,std::string actionName);

	RectData& GetData(void)
	{
		return data_;
	}

	// ‹éŒ`‚Ì•`‰æ
	void Draw(void);

private:

	RectData data_;

	Vector2I beginPos_;
	Vector2I endPos_;


	unsigned int boxColor_;
};



