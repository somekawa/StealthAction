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

    // 物理攻撃
    void NormalAttack(void);
    // スキルを使用する。
    void UseSkill(void);
    void Attack(void);
    // idleモーション
    // playerのsearch等をしている
    void Idle(void);
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
    // プレイヤーからの攻撃のくらい
    void Hit(void);
    // 死ぬ
    void Death(void);
	void(Imp::* updater_)(void);

};