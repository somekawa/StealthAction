#pragma once
#include "obj/Enemy/Enemy.h"
class Assassin :
    public Enemy
{
public:
    Assassin(cocos2d::Vector<cocos2d::Node*>& player);
    ~Assassin();

    static Assassin* CreateAssassin(cocos2d::Vector<cocos2d::Node*>& player);

    void Action(void)override;
    void update(float delta)override;
    void AnimRegistrator(void);
private:
    // 行動を決定する関数ポインタ
    void(Assassin::* updater_)(void);
    // idleモーション
    // playerのsearch等をしている
    void Idle(void);
    // runモーション
    // playerが自身の視界に入ったらplayerに向かって走る
    void Run(void);
    // attackモーション
    // playerがattack範囲に入ったらplayerを攻撃
    void Attack(void);

};

