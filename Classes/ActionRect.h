// 矩形情報ｸﾗｽ
#pragma once

#include <cocos2d.h>
#include <vector>
#include <array>
#include <string>
#include <type_traits>
#include "Geometory.h"

enum class RectType
{
	// 攻撃矩形
	Attack,
	// 攻撃当たり矩形
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

using RectArray = std::array<cocos2d::Vec2, static_cast<int>(RectType::Max)>;

struct RectData
{
	int frame_;
	Vector2I begin_;
	Vector2I end_;
	Vector2I size_;
	char type_;

	RectData() :begin_({ 0,0 }), end_({ 0,0 }),
		size_({ 0,0 }), type_(0), frame_(0) {};

	RectData(Vector2I begin, Vector2I end, Vector2I size, char type, int frame) :
		begin_(begin), end_(end), size_(size), type_(type), frame_(frame) {};
};

class ActionRect:public cocos2d::DrawNode
{
public:
	ActionRect(Vector2I begin, Vector2I end, char type, int frame);
	~ActionRect();

	cocos2d::DrawNode* CreateCollider(void);

	void Set(Vector2I begin, Vector2I end, char type, int frame, std::string actionName);

	RectData& GetData(void)
	{
		return data_;
	}

	const cocos2d::Color4F& GetColor(void) const
	{
		return boxColor_;
	}

	// ポジション取得
	const Vector2I& GetPos(void) const
	{
		return pos_;
	}
	// サイズ取得
	const Vector2I& GetSize(void) const
	{
		return data_.size_;
	}
private:

	RectData data_;

	Vector2I beginPos_;
	Vector2I endPos_;

	Vector2I pos_;
	cocos2d::Color4F boxColor_;
};