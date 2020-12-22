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

    // 物理攻撃
    void NormalAttack(void);
    // スキルを使用する。
    void UseSkill(void);
    void Attack(void);
    void Patrol(void);
    void Chase(void);
    // runモーション
    // playerが自身の視界に入ったらplayerに向かって走る
    void Run(void);
    void Walk(void);
    // ジャンプ
    void Jump(void);
    // ジャンプからの落ちるモーション
    void Fall(void);

	void(Imp::* updater_)(void);

};