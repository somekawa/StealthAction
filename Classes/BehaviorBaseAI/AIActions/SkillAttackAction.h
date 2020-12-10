#pragma once
#include "BehaviorBaseAI/ActionBase.h"

class Enemy;

class SkillAttackAction :
    public ActionBase
{
public:
    static SkillAttackAction* Instance()
    {
        static SkillAttackAction sIns_;
        return &sIns_;
    }

    virtual State Run(Enemy* enemy);
private:

};

