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
    // 物理攻撃
    void NormalAttack(void);
    // 行動を決定する関数ポインタ
    void(TwistedCultist::* updater_)(void);
};