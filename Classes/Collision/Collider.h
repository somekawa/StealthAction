#pragma once
#include <cocos2d.h>

class Collider:
	public cocos2d::Node
{
public:
	Collider(cocos2d::Sprite& target);
	~Collider();

	cocos2d::Node* Create(cocos2d::Sprite& target);

	const cocos2d::Vec2& GetPos(void) const
	{
		return pos_;
	}

	const cocos2d::Vec2& GetSize(void) const
	{
		return size_;
	}

private:
	cocos2d::Sprite& target_;
	cocos2d::Vec2 pos_;
	cocos2d::Vec2 size_;
};