#pragma once
#include <string>
#include <cocos2d.h>

class Actor;
class HPGauge :public cocos2d::Sprite
{
public:
	HPGauge();
	HPGauge(Actor& target,char moveFlag);
	~HPGauge();

	static HPGauge* createHPGauge(Actor& target, char moveFlag);
	void update(float delta);
	void SetHP(float hp);
	float GetHP(void);
private:
	float initLife_;
	float nowLife_;

	Actor* targetActor_;

	char moveFlag_;
};

