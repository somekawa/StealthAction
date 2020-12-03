#pragma once

class Enemy;

class JudgementAIBase
{
public:
	virtual bool Judgement(Enemy* enemy) = 0;
protected:
};

