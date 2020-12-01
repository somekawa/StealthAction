#pragma once
#include "BehaviorBaseAI/ActionBase.h"

class Enemy;

class MoveAction :
    public ActionBase
{
public:
    static MoveAction* Instance()
    {
        static MoveAction sIns_;
        return &sIns_;
    }

    virtual State Run(Enemy* enemy);
private:
};

