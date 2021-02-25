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