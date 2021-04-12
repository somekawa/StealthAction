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
    // �����U��
    void NormalAttack(void);
    // �s�������肷��֐��|�C���^
    void(Cultist::* updater_)(void);
    // �U����
    int attackCnt_;

	bool flag_;
	int fireBallCT_;
};