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
    void(Assassin::* updater_)(void);

    // attack���[�V����
    // player��attack�͈͂ɓ�������player���U��
    void Attack(void);

};

