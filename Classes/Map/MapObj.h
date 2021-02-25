#pragma once
#include <cocos2d.h>
class Player;
class MapObj : public cocos2d::Sprite
{
public:
	MapObj();
	virtual ~MapObj();

	virtual void Update() = 0;
	virtual bool IsHit(Player& player);
protected:
	cocos2d::Sprite* own_;
	bool isOpen_;
};

