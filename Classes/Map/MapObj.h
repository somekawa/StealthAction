#pragma once
#include <cocos2d.h>
class Player;
class MapObj : public cocos2d::Sprite
{
public:
	MapObj();
	virtual ~MapObj();

	virtual void Update(int nowEnemyNum);
	virtual bool IsHit(Player& player);
protected:
	cocos2d::Sprite* own_;
	bool isEnable_;
};

