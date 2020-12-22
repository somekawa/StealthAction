#pragma once
#include "obj/Enemy/Enemy.h"
class Assassin :
    public Enemy
{
public:
    Assassin(Player& player,
        BehaviorTree* aiTree,VisionRange visionRange,int hp);
    ~Assassin();

    static Assassin* CreateAssassin(Player& player,
        BehaviorTree* aiTree,VisionRange visionRange,int hp);

    void Action(void)override;
    void update(float delta)override;
    void AnimRegistrator(void);
    void AddAttackObj(void);
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

