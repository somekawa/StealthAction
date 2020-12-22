#include "obj/Enemy/Enemy.h"

class Imp :public Enemy
{
public:
	Imp(Player& player,
        BehaviorTree* aiTree,
        VisionRange visionRange,int hp);
	~Imp();

	static Imp* CreateImp(Player& player,
        BehaviorTree* aiTree, VisionRange visionRange,int hp);

	void Action(void)override;
	void update(float delta)override;
	void AnimRegistrator(void);

    void AddAttackObj(void);
	//CREATE_FUNC(Imp);

private:

    // �����U��
    void NormalAttack(void);
    // �X�L�����g�p����B
    void UseSkill(void);
    void Attack(void);
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

	void(Imp::* updater_)(void);

};