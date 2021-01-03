#pragma once
#include <cocos2d.h>
#include "obj/ActorType.h"
#include "BehaviorBaseAI/BehaviorTree.h"

class Player;
class Enemy;

class EnemyManager
{
public:
	EnemyManager(cocos2d::Layer& layer,cocos2d::Layer& hpLayer,Player& player);
	~EnemyManager();
	// �X�V�֐�
	void Update(void);
	// �G�֌W�̏�����
	void Initialize(void);
	// �۱�ύX�����ۂɏ��߂ɓG���݂߰��Ă���
	// param@ initNum: ���߂̓G�̐�
	void CreateInitialEnemyOnFloor(int initNum);
	
	// ���ߕ�behaviorTree�̓o�^
	void AddBehavior(ActorType type);
	// ���ߕ�spriteAnimation�̓o�^
	void AddAnim(ActorType type);
	// �G�����ߖ��ɐ���
	// �������ݸނ͓G�����񂾂�
	void CreateEnemy(ActorType type);

private:
	cocos2d::Layer& layer_;
	cocos2d::Layer& hpLayer_;
	Player* player_;
	bool spawnFlag_;

	BehaviorTree assassinBehavior_;
	BehaviorTree twistedCultistBehavior_;
	BehaviorTree cultistBehavior_;

	std::array<BehaviorTree, 5> behavior_;

	std::list<Enemy*> enemies_;
};

