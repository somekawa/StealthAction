#pragma once
#include "Geometory.h"

class Actor;
// 毎フレーム対象actorにかかる重力
#define Gravitation 0.3f

// 重力クラス
class Gravity
{
public:
	Gravity(Actor& target);
	~Gravity();
	// 重力をかける
	void ApplyGravityToTarget(float delta);
private:
	// 重力をかける対象actor
	Actor& target_;
	// 加えていく重力の力
	float g_;
};

