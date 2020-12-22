#pragma once
#include "Enemy.h"

class Cultist :
    public Enemy
{
public:
    Cultist(Player& player,
        BehaviorTree* aiTree, VisionRange visionRange, int hp);
    ~Cultist();

    static Cultist* CreateCultist(Player& player,
        BehaviorTree* aiTree, VisionRange visionRange, int hp);

    void Action(void)override;
    void update(float delta)override;
    void AnimRegistrator(void);

    void AddAttackObj(void);
private:
    // �����U��
    void NormalAttack(void);
    // �X�L�����g�p����B
    void UseSkill(void);
    // Run or walk
    void Patrol(void);
    void Chase(void);
    // run���[�V����
    // player�����g�̎��E�ɓ�������player�Ɍ������đ���
    void Run(void);
    // �W�����v
    void Jump(void);
    // �W�����v����̗����郂�[�V����
    void Fall(void);

    // �s�������肷��֐��|�C���^
    void(Cultist::* updater_)(void);
};

