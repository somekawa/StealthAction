#pragma once
#include "obj/Enemy/Enemy.h"
class Assassin :
    public Enemy
{
public:
    Assassin(cocos2d::Vector<cocos2d::Node*>& player, BehaviorTree* aiTree);
    ~Assassin();

    static Assassin* CreateAssassin(cocos2d::Vector<cocos2d::Node*>& player, BehaviorTree* aiTree);

    void Action(void)override;
    void update(float delta)override;
    void AnimRegistrator(void);
private:
    // 物理攻撃
    void NormalAttack(void);
    // スキルを使用する。
    void UseSkill(void);
    // idleモーション
    // playerのsearch等をしている
    void Idle(void);
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
    // プレイヤーからの攻撃のくらい
    void Hit(void);
    // 死ぬ
    void Death(void);

    // 行動を決定する関数ポインタ
    void(Assassin::* updater_)(void);

    // attackモーション
    // playerがattack範囲に入ったらplayerを攻撃
    void Attack(void);

};

