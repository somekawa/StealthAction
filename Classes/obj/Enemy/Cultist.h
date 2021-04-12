#pragma once
#include "Enemy.h"

class Cultist :
    public Enemy
{
public:
    Cultist(cocos2d::Vec2 pos, Player& player,
        BehaviorTree* aiTree, VisionRange visionRange, int hp,cocos2d::Layer& myLayer);
    ~Cultist();

    static Cultist* CreateCultist(cocos2d::Vec2 pos, Player& player,
        BehaviorTree* aiTree, VisionRange visionRange, int hp,cocos2d::Layer& myLayer);

    void update(float delta)override;

    bool AddAttackObj(const float& angle);
    const float GetPLAngle(void);
    void ActModuleRegistration(void);

private:
    // 物理攻撃
    void NormalAttack(void);
    // 行動を決定する関数ポインタ
    void(Cultist::* updater_)(void);
    // 攻撃回数
    int attackCnt_;

	bool flag_;
	int fireBallCT_;
};