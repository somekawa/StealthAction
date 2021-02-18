#pragma once
#include "cocos2d.h"

class PL_HPgauge:public cocos2d::Sprite
{
public:
	static cocos2d::Sprite* createPL_HPgauge();
	bool Init(void);
	void update(float delta);
	void SetHP(float hp);
	float GetHP(void);
	PL_HPgauge() = default;
	~PL_HPgauge();
private:
	CREATE_FUNC(PL_HPgauge);
	
	float initLife_;
	float nowLife_;
};

