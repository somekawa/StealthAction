#pragma once
#include "BehaviorBaseAI/JudgementAIBase.h"
#include "Geometory.h"
class Enemy;

class MoveJudgement :
    public JudgementAIBase
{
public:
    static MoveJudgement* Instance()
    {
        static MoveJudgement sIns_;
        return &sIns_;
    }

    virtual bool Judgement(Enemy* enemy);
private:

protected:
    const Vector2I visualRange_ = { 200.0f,0.0f };
};

