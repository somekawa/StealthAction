#pragma once
#include "Enemy.h"
class TwistedCultist :
    public Enemy
{
public:
    TwistedCultist(cocos2d::Vector<cocos2d::Node*>& player);
    ~TwistedCultist();

    static TwistedCultist* CreateTwistedCultist(cocos2d::Vector<cocos2d::Node*>& player);

    void Action(void)override;
    void update(float delta)override;
    void AnimRegistrator(void);
private:
    // �s�������肷��֐��|�C���^
    void(TwistedCultist::* updater_)(void);
    // idle
    void Idle(void);
};

