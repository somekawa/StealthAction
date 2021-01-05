#pragma once
#include <cocos2d.h>
#include "obj/ActorType.h"
#include "BehaviorBaseAI/BehaviorTree.h"

class Player;
class Enemy;
class EffectManager;

class EnemyManager
{
public:
	EnemyManager(cocos2d::Layer& layer,cocos2d::Layer& hpLayer,Player& player);
	~EnemyManager();
	// �X�V�֐�
	void Update(const std::shared_ptr<EffectManager>& effectMng);
	// �G�֌W�̏�����
	void Initialize(void);
	// �۱�ύX�����ۂɏ��߂ɓG���݂߰��Ă���
	// param@ initNum: ���߂̓G�̐�
	void CreateInitialEnemyOnFloor(int initNum, const std::shared_ptr<EffectManager>& effectMng);
	
	// ���ߕ�behaviorTree�̓o�^
	void AddBehavior(ActorType type);
	// ���ߕ�spriteAnimation�̓o�^
	void AddAnim(ActorType type);
	// �G�����ߖ��ɐ���
	// �������ݸނ͓G�����񂾂�
	void CreateEnemy(ActorType type,const std::shared_ptr<EffectManager>& effectMng);
	// �޽�̏o���֐�
	void CreateBoss(const std::shared_ptr<EffectManager>& effectMng);
private:
	cocos2d::Layer& layer_;
	cocos2d::Layer& hpLayer_;

	Player* player_;
	bool spawnFlag_;

	std::array<BehaviorTree, 6> behavior_;

	std::list<Enemy*> enemies_;
};

