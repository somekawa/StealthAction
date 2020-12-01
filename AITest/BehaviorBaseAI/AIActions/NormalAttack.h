#pragma once
#include "BehaviorBaseAI/ActionBase.h"

class Enemy;

class NormalAttack :
    public ActionBase
{
public:
    static NormalAttack* Instance(void)
    {
        static NormalAttack sIns_;
        return &sIns_;
    }

    virtual State Run(Enemy* enemy);
private:

};

