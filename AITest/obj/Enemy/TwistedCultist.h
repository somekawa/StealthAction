#pragma once
#include "Enemy.h"
class TwistedCultist :
    public Enemy
{
public:
    TwistedCultist(cocos2d::Vector<cocos2d::Node*>& player, BehaviorTree* aiTree);
    ~TwistedCultist();

    static TwistedCultist* CreateTwistedCultist(cocos2d::Vector<cocos2d::Node*>& player, BehaviorTree* aiTree);

    void Action(void)override;
    void update(float delta)override;
    void AnimRegistrator(void);
private:
    // �����U��
    void NormalAttack(void);
    // �X�L�����g�p����B
    void UseSkill(void);
    // idle���[�V����
    // player��search�������Ă���
    void Idle(void);
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
    // �v���C���[����̍U���̂��炢
    void Hit(void);
    // ����
    void Death(void);
    // �s�������肷��֐��|�C���^
    void(TwistedCultist::* updater_)(void);
};

