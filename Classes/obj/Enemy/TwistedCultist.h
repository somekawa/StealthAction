#pragma once
#include "Enemy.h"
class TwistedCultist :
    public Enemy
{
public:
    TwistedCultist(Player& player,
        BehaviorTree* aiTree, VisionRange visionRange,int hp);
    ~TwistedCultist();

    static TwistedCultist* CreateTwistedCultist(Player& player,
        BehaviorTree* aiTree, VisionRange visionRange,int hp);

    void Action(void)override;
    void update(float delta)override;
    void AnimRegistrator(void);
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
    void(TwistedCultist::* updater_)(void);
};

