#pragma once
#include <string>
#include <cocos2d.h>

class Actor;

class HPGauge :public cocos2d::Sprite
{
public:
	HPGauge(Actor& target,char moveFlag);

	static cocos2d::Sprite* createHPGauge(Actor& target, char moveFlag);
	bool Init(void);
	void update(float delta);
	void SetHP(float hp);
	float GetHP(void);
private:
	~HPGauge();
	float initLife_;
	float nowLife_;

	Actor& targetActor_;

	char moveFlag_;
};

