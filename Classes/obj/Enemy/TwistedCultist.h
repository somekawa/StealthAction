#pragma once
#include "Enemy.h"
class TwistedCultist :
    public Enemy
{
public:
    TwistedCultist(Player& player,
        BehaviorTree* aiTree, VisionRange visionRange,int hp,cocos2d::Layer& myLayer);
    ~TwistedCultist();

    static TwistedCultist* CreateTwistedCultist(Player& player,
        BehaviorTree* aiTree, VisionRange visionRange,int hp,cocos2d::Layer& myLayer);

    void Action(void)override;
    void update(float delta)override;
    void AnimRegistrator(void);

    void AddAttackObj(const float& angle);
private:
    // 物理攻撃
    void NormalAttack(void);
    // スキルを使用する。
    void UseSkill(void);
    // Run or walk
    void Patrol(void);
    void Chase(void);
    // runモーション
    // playerが自身の視界に入ったらplayerに向かって走る
    void Run(void);
    // ジャンプ
    void Jump(void);
    // ジャンプからの落ちるモーション
    void Fall(void);
    // 行動を決定する関数ポインタ
    void(TwistedCultist::* updater_)(void);
};

