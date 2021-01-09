#pragma once
#include "obj/Enemy/Enemy.h"
class Assassin :
    public Enemy
{
public:
    Assassin(cocos2d::Vec2 pos,Player& player,
        BehaviorTree* aiTree,VisionRange visionRange,int hp,cocos2d::Layer& myLayer);
    ~Assassin();

    static Assassin* CreateAssassin(cocos2d::Vec2 pos, Player& player,
        BehaviorTree* aiTree,VisionRange visionRange,int hp,cocos2d::Layer& myLayer);

    void Action(void)override;
    void update(float delta)override;
    void AnimRegistrator(void);
    void AddAttackObj(const float& angle);

    void actModuleRegistration(void);
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
    void(Assassin::* updater_)(void);

    // attackモーション
    // playerがattack範囲に入ったらplayerを攻撃
    void Attack(void);

};

