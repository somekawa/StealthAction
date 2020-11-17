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
    // �s�������肷��֐��|�C���^
    void(Assassin::* updater_)(void);
    // idle���[�V����
    // player��search�������Ă���
    void Idle(void);
    // run���[�V����
    // player�����g�̎��E�ɓ�������player�Ɍ������đ���
    void Run(void);
    // attack���[�V����
    // player��attack�͈͂ɓ�������player���U��
    void Attack(void);

};

