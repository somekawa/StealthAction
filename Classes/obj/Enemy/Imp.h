#include "obj/Enemy/Enemy.h"

class Imp :public Enemy
{
public:
	Imp(cocos2d::Vec2 pos, Player& player,
        BehaviorTree* aiTree,
        VisionRange visionRange,int hp,cocos2d::Layer& myLayer);
	~Imp();

	static Imp* CreateImp(cocos2d::Vec2 pos, Player& player,
        BehaviorTree* aiTree, VisionRange visionRange,int hp,cocos2d::Layer& myLayer);

	void Action(void)override;
	void update(float delta)override;
	void AnimRegistrator(void);

    bool AddAttackObj(const float& angle);
	//CREATE_FUNC(Imp);
    void actModuleRegistration(void);

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