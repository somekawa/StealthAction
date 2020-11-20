#pragma once
#include "cocos2d.h"
#include <memory>

class Player;
class Gate : public cocos2d::Sprite
{
public:
	Gate(cocos2d::Vec2 pos, int gateNum);
	~Gate() = default;
	static Gate* CreateGate(cocos2d::Vec2 pos,int gateNum);

	void Update(Player& player);
	bool IsHit(Player& player);
	int GetGateNum();
private:
	cocos2d::Vec2 pos_;
	Sprite* own_;
	int gateNum_;
	bool isOpen_;
	int frame = 0;

};

