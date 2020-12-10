#pragma once
#include "BehaviorBaseAI/JudgementAIBase.h"

class Enemy;

class SearchJudgement :
    public JudgementAIBase
{
public:
    static SearchJudgement* Instance(void)
    {
        static SearchJudgement sIns_;
        return &sIns_;
    }

    virtual bool Judgement(Enemy* enemy);
private:

};

