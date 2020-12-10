#pragma once
#include "BehaviorBaseAI/ActionBase.h"

class Enemy;

class SearchAction :
    public ActionBase
{
public:
    static SearchAction* Instance(void)
    {
        static SearchAction sIns_;
        return &sIns_;
    }

    virtual State Run(Enemy* enemy);
private:

};

