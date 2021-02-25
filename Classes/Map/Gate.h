#pragma once
#include "MapObj.h"
#include <memory>

class Player;
class Gate : public MapObj
{
public:
	Gate(cocos2d::Vec2 pos, int gateNum);
	~Gate();
	static Gate* CreateGate(cocos2d::Vec2 pos, int gateNum);

	void Update()override;
	//bool IsHit(Player& player)override;
	int GetGateNum();
private:
	int gateNum_;
	int frame = 0;
};

