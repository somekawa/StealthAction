// ��`���׽
#pragma once

#include <cocos2d.h>
#include <vector>
#include <array>
#include <string>
#include <type_traits>
#include "Geometory.h"

enum class RectType
{
	// �U����`
	Attack,
	// �U���������`
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
	// ���݂����ڰѐ�
	int frame_;
	// ��`�̎n�_
	Vector2I begin_;
	// ��`�̏I�_
	Vector2I end_;
	// ��`�̻���
	Vector2I size_;
	// ��`������(�U����`�����炢��`��)
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
	// �ײ�ް�̐���
	ActionRect* CreateCollider(void);
	// �ײ�ް�̾��
	void Set(Vector2I begin, Vector2I end, char type, int frame, std::string actionName);
	// �ײ�ް���ް��擾
	RectData& GetData(void)
	{
		return data_;
	}
	// �ײ�ް�̐F�擾(�U����`:�� ���炢��`:��)
	const cocos2d::Color4F& GetColor(void) const
	{
		return boxColor_;
	}

	// �|�W�V�����擾
	const Vector2I& GetPos(void) const
	{
		return pos_;
	}
	// �T�C�Y�擾
	const Vector2I& GetSize(void) const
	{
		return data_.size_;
	}
private:
	// �e�ް�
	// �n�_�A�I�_�A�ڰсA���߁A����
	RectData data_;
	// �n�_
	Vector2I beginPos_;
	// �I�_
	Vector2I endPos_;
	// ��`���߼޼��
	Vector2I pos_;
	// ��`�̐F
	// �U����`:��
	// ���炢��`:��
	cocos2d::Color4F boxColor_;
};