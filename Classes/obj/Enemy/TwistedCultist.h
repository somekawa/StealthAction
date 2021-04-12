#pragma once
#include "Enemy.h"

class TwistedCultist :
    public Enemy
{
public:
    TwistedCultist(cocos2d::Vec2 pos,Player& player,
        BehaviorTree* aiTree, VisionRange visionRange,int hp,cocos2d::Layer& myLayer);
    ~TwistedCultist();
    static TwistedCultist* CreateTwistedCultist(cocos2d::Vec2 pos, Player& player,
        BehaviorTree* aiTree, VisionRange visionRange,int hp,cocos2d::Layer& myLayer);

    void update(float delta)override;

    bool AddAttackObj(const float& angle);
    void ActModuleRegistration(void);

private:
    // �����U��
    void NormalAttack(void);
    // �s�������肷��֐��|�C���^
    void(TwistedCultist::* updater_)(void);
};