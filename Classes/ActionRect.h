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
	// 存在するﾌﾚｰﾑ数
	int frame_;
	// 矩形の始点
	Vector2I begin_;
	// 矩形の終点
	Vector2I end_;
	// 矩形のｻｲｽﾞ
	Vector2I size_;
	// 矩形のﾀｲﾌﾟ(攻撃矩形かくらい矩形か)
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
	// ｺﾗｲﾀﾞｰの生成
	ActionRect* CreateCollider(void);
	// ｺﾗｲﾀﾞｰのｾｯﾄ
	void Set(Vector2I begin, Vector2I end, char type, int frame, std::string actionName);
	// ｺﾗｲﾀﾞｰのﾃﾞｰﾀ取得
	RectData& GetData(void)
	{
		return data_;
	}
	// ｺﾗｲﾀﾞｰの色取得(攻撃矩形:赤 くらい矩形:青)
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
	// 各ﾃﾞｰﾀ
	// 始点、終点、ﾌﾚｰﾑ、ﾀｲﾌﾟ、ｻｲｽﾞ
	RectData data_;
	// 始点
	Vector2I beginPos_;
	// 終点
	Vector2I endPos_;
	// 矩形のﾎﾟｼﾞｼｮﾝ
	Vector2I pos_;
	// 矩形の色
	// 攻撃矩形:赤
	// くらい矩形:青
	cocos2d::Color4F boxColor_;
};