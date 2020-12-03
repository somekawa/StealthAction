#include "obj/Enemy/Enemy.h"

class Imp :public Enemy
{
public:
	Imp(cocos2d::Vector<cocos2d::Node*>& player, BehaviorTree* aiTree);
	~Imp();

	static Imp* CreateImp(cocos2d::Vector<cocos2d::Node*>& player, BehaviorTree* aiTree);

	void Action(void)override;
	void update(float delta)override;
	void AnimRegistrator(void);
	//CREATE_FUNC(Imp);

private:

    // �����U��
    void NormalAttack(void);
    // �X�L�����g�p����B
    void UseSkill(void);
    void Attack(void);
    // idle���[�V����
    // player��search�������Ă���
    void Idle(void);
    void Patrol(void);
    void Chase(void);
    // run���[�V����
    // player�����g�̎��E�ɓ�������player�Ɍ������đ���
    void Run(void);
    void Walk(void);
    // �W�����v
    void Jump(void);
    // �W�����v����̗����郂�[�V����
    void Fall(void);
    // �v���C���[����̍U���̂��炢
    void Hit(void);
    // ����
    void Death(void);
	void(Imp::* updater_)(void);

};