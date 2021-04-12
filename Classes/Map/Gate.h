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
	int GetGateNum();
private:
	int gateNum_;
};